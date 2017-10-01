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
	//Liga e desliga o LED verde do RGB
	kit.ledRGB(0, 100, 0);
	delay(1000);
	kit.ledRGB(0, 0, 0);
	delay(1000);

	//Varia a intensidade de 100 a 0 e de 0 a 100 do LED azul
	for (int i = -100; i <= 100; i++) {
		kit.ledRGB(0, 0, abs(i));
		delay(30);
	}

}
