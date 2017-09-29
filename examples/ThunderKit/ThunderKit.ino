#include "ThunderKit.h"

// Declara o kit, necessario passar o numero
ThunderKit kit(1);

void setup() {
	if (kit.begin() != 0) {
		Serial.println("Erro na inicialização :(");
		while(1);
	}
	kit.ledArcoIris();
}

void loop() {
	uint8_t envio[] = { 255, 0, 127, 0, 128, 254 };
	Serial1.write(envio, 6);

	if (Serial1.available())
		Serial.println(Serial1.readString());

	delay(100);

	// kit.setSpeed(50, 50);
	// delay(5000);
	// kit.setSpeed(-50, -50);
	// delay(5000);

	// delay(1000);
	// Serial.println("Up!");
	// for(int i = -100; i <= 100; i++) {
	// 	kit.setSpeed(i, i);
	// 	analogWrite(5, abs(i));
	// 	delay(10);
	// }
}

