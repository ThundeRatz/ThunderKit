#include "ThunderKit.h"

/*
 * Exemplo de uso geral do ThunderKit
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
	// kit.ledArcoIris();
	// kit.ativarMotores();
}

void loop() {

	// Recebe os comandos pelo aplicativo
	kit.appCommand();

	// Se iniciar o modo autonomo pelo app,
	// passa a se comportar como seguidor de liha
	if (kit.seguidor())
		seguirLinha();

	// Se receber informacao do joystick,
	// manda os motores de moverem
	if (kit.joystick() > 0)
		mover();

	delay(10);
}

void seguirLinha() {
	Serial.print("Seguindo a linha!");
}

void mover() {

	// Obtem as informacoes enviadas pelo app
	int direcao = kit.joystick(DIRECAO);
	int velocidade = kit.joystick(VELOCIDADE);

	// De acordo com a velocidade selecionada no app,
	// calibra o sinal para os motores
	if (velocidade == 1)
		velocidade = 30;
	else if (velocidade == 2)
		velocidade = 60;
	else if (velocidade == 3)
		velocidade = 100;

	// De acordo com a posicao do joystick,
	// configura velocidades diferentes para os motores
	if (direcao == 1)      // Frente
		kit.Motores(velocidade, velocidade);
	else if (direcao == 2) // Frente Direita
		kit.Motores(velocidade, velocidade/3);
	else if (direcao == 3) // Direita
		kit.Motores(velocidade, -velocidade);
	else if (direcao == 4) // Tras Direita
		kit.Motores(-velocidade, -velocidade/3);
	else if (direcao == 5) // Tras
		kit.Motores(-velocidade, -velocidade);
	else if (direcao == 6) // Tras Esquerda
		kit.Motores(-velocidade/3, -velocidade);
	else if (direcao == 7) // Esquerda
		kit.Motores(-velocidade, velocidade);
	else if (direcao == 8) // Frente Esquerda
		kit.Motores(velocidade/3, velocidade);
}
