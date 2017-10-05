#include "Arduino.h"
#include "ThunderKit.h"

const int __sensors[] = { A1, A2, A3, A4, A5 };

ThunderKit::ThunderKit(int kit_number) {
	sprintf(at_name, "AT+NAMBThunderKit%d", kit_number);
	motors_on = false;
	seguidor_on = false;
	joystick_pos = 0;
}

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

int ThunderKit::joystick() {
	return joystick_pos;
}

int ThunderKit::joystick(int parte) {
	if (parte == DIRECAO)
		return joystick_pos/10;

	if (parte == VELOCIDADE)
		return joystick_pos%10;

	return 0;
}


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
	Recebe: Numero do sensor| limiar a ser designado
*/
void ThunderKit::setThreshold(int posicao, int threshold) {
	sensors[posicao].limiar = threshold;
}

/*
	Recebe: Numero do sensor
	Retorna: 0 - caso abaixo do limiar (preto) | 1 - caso acima do limiar (branco) | -1 - caso num_sensor invalido
*/
int ThunderKit::getColor(int posicao) {
	uint16_t reading = analogRead(sensors[posicao].pino);
	return (reading >= sensors[posicao].limiar);
}

/*
	Recebe: Numero do sensor
	Retorna: Leitura analogica do pino
*/
int ThunderKit::lerSensor(int posicao) {
	if (posicao > 5 || posicao < 0)
		return -1;

	if (sensors[posicao].pino == uint8_t(-1))
		return -1;

	return analogRead(sensors[posicao].pino);
}

void ThunderKit::ativarMotores() {
	analogWrite(LED, 0);
	analogWrite(AIN2, 0);
}

void ThunderKit::desativarMotores() {
	analogWrite(AIN1, 0);
	analogWrite(BIN1, 0);
	analogWrite(AIN2, 0);
	analogWrite(BIN2, 0);
}

/*
	Recebe: vel_esq (-100 -> 100) | vel_dir (-100 -> 100)
	vel_esq	-> Velocidade do motor esquerdo
	vel_dir	-> Qual o sentido de rotacao

	De acordo com a pg 9 do datasheet http://www.ti.com/lit/ds/symlink/drv8833.pdf
 */
void ThunderKit::setSpeed(int vel_esq, int vel_dir) {
	if (!motors_on)
		return;

	vel_esq = constrain(vel_esq, -100, 100);
	vel_dir = constrain(vel_dir, -100, 100);

	vel_esq = map(vel_esq, -100, 100, -255, 255);
	vel_dir = map(vel_dir, -100, 100, -255, 255);

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
}

// Acende o LED verde na intensidade especificada (entre 0 e 100)
void ThunderKit::ledVerde(int intensidade) {
	intensidade = map(constrain(intensidade, 0, 100), 0, 100, 0, 255);
	analogWrite(LEDG, 255 - intensidade);
	analogWrite(LEDR, 0);  // Pino de alimentacao do LED precisa estar ligado
}

// Acende o LED azul na intensidade especificada (entre 0 e 100)
void ThunderKit::ledAzul(int intensidade) {
	intensidade = map(constrain(intensidade, 0, 100), 0, 100, 0, 255);
	analogWrite(LEDB, 255 - intensidade);
	analogWrite(LEDR, 0);  // Pino de alimentacao do LED precisa estar ligado
}

// Muda as intensidades dos LEDs verde e azul aleatoriamente por 5 segundos
void ThunderKit::ledArcoIris() {
	for (uint16_t i = 0; i < 500; i++) {
		ledVerde(random(100));
		ledAzul(random(100));
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
