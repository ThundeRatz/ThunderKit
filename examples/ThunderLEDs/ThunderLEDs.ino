#include "ThunderKit.h"

/*
 * Exemplo de uso do LED RGB do ThunderKit
 */

// Declara o kit, necessario passar o numero
// para nomear o dispositivo Bluetooth

ThunderKit kit(1, 0);

void setup() {

	// Inicia a biblioteca do kit, aborta a execucao se falhar
	if (kit.begin() != 0) {
		Serial.println("Erro na inicialização :(");
		while(1);
	}
}

void loop() {

	//Liga e desliga o LED verde
	kit.ledVerde(100);
	delay(1000);

	kit.ledVerde(0);
	delay(1000);

	//Varia a intensidade de 100 a 0 e de 0 a 100 do LED azul
	for (int i = -100; i <= 100; i++) {
		kit.ledAzul(abs(i));
		delay(30);
	}

}
