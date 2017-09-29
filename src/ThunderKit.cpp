#include "Arduino.h"
#include "ThunderKit.h"

const int __sensors[] = { A1, A2, A3, A4, A5 };

ThunderKit::ThunderKit(int kit_number) {
	sprintf(at_name, "AT+NAMBThunderKit%d", kit_number);
	motors_on = false;
	modo_atual = RC;
}

int ThunderKit::begin() {
	Serial.begin(9600);
	Serial.println("ThunderKit started!");

	// Bluetooth setup
	Serial.println("Configurando tudo...");
	Serial.println("-- Bluetooh");
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
	motors_on = true;

	analogWrite(LED, 0);
	analogWrite(AIN2, 0);
}

void ThunderKit::desativarMotores() {
	if (!motors_on)
		return;

	analogWrite(AIN1, 0);
	analogWrite(BIN1, 0);
	analogWrite(AIN2, 0);
	analogWrite(BIN2, 0);

	motors_on = false;
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

void ThunderKit::ledRGB(int r, int g, int b) {
	r = map(constrain(r, 0, 100), 0, 100, 0, 255);
	g = map(constrain(r, 0, 100), 0, 100, 0, 255);
	b = map(constrain(r, 0, 100), 0, 100, 0, 255);

	analogWrite(LEDR, 255 - r);
	analogWrite(LEDG, 255 - g);
	analogWrite(LEDB, 255 - b);
}

void ThunderKit::led(int intensidade) {
	if (motors_on)
		return;

	// Escrevea mesma PWM nos dois pinos
	// para impedir o motor de girar com
	// essa funcao
	analogWrite(LED, intensidade);
	analogWrite(AIN2, intensidade);
}

void ThunderKit::ledArcoIris() {
	// Muda as cores aleatoriamente por 5 segundos
	for (uint16_t i = 0; i < 50000; i++) {
		ledRGB(random(100), random(100), random(100));
		delay(10);
	}
}

