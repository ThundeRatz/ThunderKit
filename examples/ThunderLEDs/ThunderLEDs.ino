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
	kit.ligarLED(LEDG);
	delay(1000);
	kit.desligarLED(LEDG);
	delay(1000);

	//Varia a intensidade de 0 a 100 do LED azul
	for (int i = 0; i <= 100; i++) {
		kit.ligarLED(LEDB, i);
		delay(100);
	}

}
