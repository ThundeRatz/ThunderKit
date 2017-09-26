#include "ThunderKit.h"

// Declara o kit, necessario passar o numero
ThunderKit kit(1);

void setup() {
	if (kit.begin() != 0) {
		Serial.println("Erro na inicialização :(");
		while(1);
	}
}

void loop() {
	uint8_t envio[] = { 255, 0, 127, 0, 128, 254 };
	Serial1.write(envio , 6);

	if (Serial1.available())
		Serial.println(Serial1.readString());

	delay(1000);
}
