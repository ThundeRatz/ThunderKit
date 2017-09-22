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
	if (Serial1.available())
		Serial.println(Serial1.readString());
}
