#include "Arduino.h"
#include "ThunderKit.h"

static const int __sensors[] = { A1, A2, A3, A4, A5 };

// Forneca um numero para o construtor para sua identificacao bluetooth
ThunderKit::ThunderKit(int kit_number, int _kind) {
	kind = _kind;
	sprintf(at_name, "AT+NAMBThunderKit%d", kit_number);
	seguidor_on = false;
	joystick_pos = 0;
}

// Faz a configuracoes iniciais necessarias
int ThunderKit::begin() {
	Serial.begin(9600);
	Serial.println("ThunderKit iniciado!");

	// Bluetooth setup
	Serial.println("Configurando tudo...");
	Serial.println("-- Bluetooth");
	delay(1000);

	for (;;) {
		delay(500);
		Serial1.begin(9600);
		delay(500);
		Serial.println("try 9600...");
		if (send_msg("AT") == 0) {
			if (strcmp(recv_str, "OK") == 0)
				break;
		}
		Serial.println(" NO");

		delay(500);
		Serial1.begin(115200);
		delay(500);
		Serial.println("try 115200...");
		if (send_msg("AT") == 0) {
			if (strcmp(recv_str, "OK") == 0)
				break;
		}
		Serial.println(" NO");
	}

	// Reconfigurar Bluetooth
	send_msg("AT+RENEW");
	send_msg("AT+BAUD2");
	send_msg("AT+AUTH0");
	send_msg("AT+RESET");
	Serial1.begin(9600);
	delay(2000);

	send_msg("AT+VERS?");
	send_msg("AT+ADDB?");
	send_msg(at_name);
	send_msg("AT+SCAN0");
	send_msg("AT+NOTI1");
	send_msg("AT+NOTP0");
	send_msg("AT+ROLB0");

	send_msg("AT+RESET");
	delay(2000);

	if (send_msg("AT") != 0)
		if (strcmp(recv_str, "OK") == 0)
			return -1;

	Serial.println("-- Bluetooth OK");
	Serial.println();

	// Sensores
	Serial.println("-- Sensores");
	for (int i = 0; i < 5; i++) {
		sensors[i].pino = __sensors[i];
		sensors[i].limiar = 512;
		pinMode(__sensors[i], INPUT);
		Serial.print("Sensor ");
		Serial.print(i);
		Serial.println(" OK");
	}
	Serial.println("-- Sensores OK");
	Serial.println();

	Serial.println("Configuração concluida!");

	return 0;
}

// Recebe os comandos pelo aplicativo
int ThunderKit::appCommand() {
	if (!Serial1.available())
		return -2;

	int temp = Serial1.read();

	// Serial.println(temp);
	if (temp >= 0 && temp < 84) {
		joystick_pos = temp;
		seguidor_on = false;
	} else {
		seguidor_on = temp == 200 ? true : false;
		joystick_pos = 0;
	}

	return 0;
}

// Retorna a posicao do joystick (de 0 a 83)
int ThunderKit::joystick() {
	return joystick_pos;
}

/* Se o tipo for DIRECAO, retorna a direcao (0 a 8) do joystick.
   Se o tipo for VELOCIDADE, retorna a velocidade (0, 1, 2 ou 3) do joystick */
int ThunderKit::joystick(int tipo) {
	if (tipo == DIRECAO)
		return joystick_pos/10;

	if (tipo == VELOCIDADE)
		return joystick_pos%10;

	return 0;
}

// Retorna verdadeiro se estiver no modo seguidor de linha
boolean ThunderKit::seguidor() {
	return seguidor_on;
}

int ThunderKit::recv_msg(int timeout) {
	uint16_t time = millis();
	uint8_t i = 0;

	for (;;) {
		if (Serial1.available()) {
			recv_str[i++] = char(Serial1.read());
			break;
		}
		if (millis() - time > timeout)
			return -1;
	}

	delay(30);

	while (Serial1.available() && i < 100) {
		recv_str[i++] = char(Serial1.read());
	}

	recv_str[i] = '\0';

	return 0;
}

int ThunderKit::send_msg(const String& msg) {
	Serial.print("Enviando: ");
	Serial.println(msg);

	Serial1.print(msg);
	delay(1);

	if (recv_msg(500) != 0)
		return -1;

	Serial.print("Recebido: ");
	Serial.println(recv_str);

	return 0;
}

/*
	Recebe: Numero do sensor e limiar a ser designado
*/
void ThunderKit::definirLimiar(int posicao, int limiar) {
	sensors[posicao].limiar = limiar;
}

/*
	Recebe: Numero do sensor
	Retorna: Verdadeiro se a leitura do sensor for branca, falso se for preta
*/
boolean ThunderKit::branco(int posicao) {
	uint16_t reading = lerSensor(posicao);
	return (reading >= sensors[posicao].limiar);
}

/*
	Recebe: Numero do sensor
	Retorna: Leitura analogica do pino (de 0 a 1023)
*/
int ThunderKit::lerSensor(int posicao) {
	if (posicao > 5 || posicao < 0)
		return -1;

	if (sensors[posicao].pino == uint8_t(-1))
		return -1;

	return analogRead(sensors[posicao].pino);
}

/*
	Recebe: Velocidades dos motores direito e esquerdo (ambos de -100 a 100)
	Escolhe a velocidade dos motores.
 */
void ThunderKit::motores(int vel_esq, int vel_dir) {

	vel_esq = constrain(vel_esq, -100, 100);
	vel_dir = constrain(vel_dir, -100, 100);

	vel_esq = map(vel_esq, -100, 100, -255, 255);
	vel_dir = map(vel_dir, -100, 100, -255, 255);

	if(kind == 0){
		if (vel_esq > 0) {
			analogWrite(AIN1, vel_esq);
			analogWrite(AIN2, 0);
		} else {
			analogWrite(AIN1, 0);
			analogWrite(AIN2, abs(vel_esq));
		}

		if (vel_dir > 0) {
			analogWrite(BIN2, vel_dir);
			analogWrite(BIN1, 0);
		} else {
			analogWrite(BIN2, 0);
			analogWrite(BIN1, abs(vel_dir));
		}
	}else if(kind == 1){

		analogWrite(AIN1, abs(vel_esq));
		if (vel_esq > 0) {
			digitalWrite(AIN2, HIGH);
			digitalWrite(AIN3, LOW);
		} else {
			digitalWrite(AIN2, LOW);
			digitalWrite(AIN3, HIGH);
		}

		analogWrite(BIN1, abs(vel_dir));
		if (vel_dir > 0) {
			digitalWrite(BIN2, HIGH);
			digitalWrite(BIN3, LOW);
		} else {
			digitalWrite(BIN2, LOW);
			digitalWrite(BIN3, HIGH);
		}
	}
}

// Acende o LED verde na intensidade especificada (entre 0 e 100)
void ThunderKit::ledVerde(int intensidade) {
	intensidade = map(constrain(intensidade, 0, 100), 0, 100, 0, 255);
	analogWrite(LEDG, 255 - intensidade);
	analogWrite(LEDR, 255);  // Pino de alimentacao do LED precisa estar ligado
}

// Acende o LED azul na intensidade especificada (entre 0 e 100)
void ThunderKit::ledAzul(int intensidade) {
	intensidade = map(constrain(intensidade, 0, 100), 0, 100, 0, 255);
	analogWrite(LEDB, 255 - intensidade);
	analogWrite(LEDR, 255);  // Pino de alimentacao do LED precisa estar ligado
}

// Muda as intensidades dos LEDs verde e azul gradualmente e volta
void ThunderKit::ledFade() {
	ledVerde(0);
	ledAzul(0);

	for (int i = -100; i <= 100; i++) {
		ledVerde(100 - abs(i));
		delay(10);
	}

	ledVerde(0);
	delay(100);

	for (int i = -100; i <= 100; i++) {
		ledAzul(100 - abs(i));
		delay(10);
	}
}

// Envia a leitura dos sensores para o app
void ThunderKit::enviarSensores() {
	uint16_t message = 0;
	uint8_t msg_bytes[2];
	for (int i = 0; i < 5; i++) {
		message = 0 << 15 | i << 12 | lerSensor(i) << 1;
		msg_bytes[0] = message & 0xFF;
		msg_bytes[1] = message >> 8;
		Serial.println(message);
		Serial1.write(msg_bytes, 2);
		delay(100);
	}
}
