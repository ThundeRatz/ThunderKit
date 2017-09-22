#include "Arduino.h"
#include "Wire.h"

#include <string.h>

#include "ThunderKit.h"

ThunderKit::ThunderKit(int kit_number) {
	sprintf(at_name, "AT+NAMBThunderKit%d", kit_number);
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
		if(send_msg("AT") == 0) {
			break;
		}
		Serial.println(" NO");

		delay(500);
		Serial1.begin(115200);
		delay(500);
		Serial.println("try 115200...");
		if(send_msg("AT") == 0) {
			break;
		}
		Serial.println(" NO");
	}

	// Reconfigurar Bluetooth
	send_msg("AT+RENEW");
	send_msg("AT+BAUD2");
	send_msg("AT+AUTH0");
	send_msg("AT+RESET");
	Serial1.begin(9500);
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
		return -1;

	Serial.println("-- Bluetooth OK");
	Serial.println();

	// Confugracao motores e outras coisas
	Serial.println("configuração concluida!");

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

int ThunderKit::send_msg(char* msg) {
	Serial.print("Enviando: ");
	Serial.println(msg);

	Serial1.print(msg);
	delay(1);

	if(recv_msg(500) != 0)
		return -1;

	Serial.print("Recebido: ");
	Serial.println(recv_str);

	return 0;
}
