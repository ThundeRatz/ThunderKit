#include "ThunderKit.h"

/*
 * Exemplo de uso do ThunderKit com motores DC
 */


// Declara o kit, necessario passar o numero
// para nomear o dispositivo Bluetooth


ThunderKit kit(1);

void setup() {
	// Inicia a biblioteca do kit, aborta a execucao de falhar
	if (kit.begin() != 0) {
		Serial.println("Erro na inicialização :(");
		while(1);
	}
}

void loop() {
	// Demonstracao do funcionamento dos motores

	// Acelera lentamente os motores
	// no sentido horario, com 80% de velocidade
	kit.setSpeed(80, 80);
	delay(2000);

	// Desacelera lentamente os motores
	// no sentido horario, ate velocidade zero
	kit.setSpeed(0, 0);
	delay(1000);

	// Acelera rapidamente os motores
	// no sentido anti-horario, com 50% de velocidade
	kit.setSpeed(50, 50);
	delay(2000);

	// Para os dois motores rapidamente
	kit.stopAll();

	delay(10000);
}
