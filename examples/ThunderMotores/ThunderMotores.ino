#include "ThunderKit.h"

/*
 * Exemplo de uso do ThunderKit com motores DC
 */


// Declara o kit, necessario passar o numero
// para nomear o dispositivo Bluetooth

ThunderKit kit(1);

void setup() {

	// Inicia a biblioteca do kit, aborta a execucao se falhar
	if (kit.begin() != 0) {
		Serial.println("Erro na inicialização :(");
		while(1);
	}
}

void loop() {

	// Demonstracao do funcionamento dos motores

	// Inicia o modo de controle dos motores
	kit.ativarMotores();

	// Acelera os motores para frente,
	// com 80% de velocidade
	kit.motores(80, 80);
	delay(5000);

	// Para os motores completamente
	kit.motores(0, 0);
	delay(1000);

	// Acelera motores para trás,
	// com 50% de velocidade
	kit.motores(-50, -50);
	delay(5000);

	// Para o motor esquerdo
	kit.motores(0, -50);
	delay(5000);

	// Curva para a esquerda
	kit.motores(20, 60);
	delay(5000);

	// Para os dois motores
	kit.desativarMotores();

	delay(10000);
}
