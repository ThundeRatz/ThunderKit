#include "ThunderKit.h"

// Declara o kit, necessario passar o numero
ThunderKit kit(1);

void setup() {
	if (kit.begin() != 0) {
		Serial.println("Erro na inicialização :(");
		while(1);
	}
	// kit.ledArcoIris();
	// kit.ativarMotores();
}

void loop() {
	kit.appCommand();

	if (kit.seguidor())
		seguirLinha();

	if (kit.joystick() > 0)
		mover();

	delay(10);
}

void seguirLinha() {
	Serial.print("Seguindo a linha!");
}

void mover() {
	int direcao = kit.joystick(DIRECAO);
	int velocidade = kit.joystick(VELOCIDADE);

	if (velocidade == 1)
		velocidade = 30;
	else if (velocidade == 2)
		velocidade = 60;
	else if (velocidade == 3)
		velocidade = 100;

	if (direcao == 1)      // Frente
		kit.setSpeed(velocidade, velocidade);
	else if (direcao == 2) // Frente Direita
		kit.setSpeed(velocidade, velocidade/3);
	else if (direcao == 3) // Direita
		kit.setSpeed(velocidade, -velocidade);
	else if (direcao == 4) // Tras Direita
		kit.setSpeed(-velocidade, -velocidade/3);
	else if (direcao == 5) // Tras
		kit.setSpeed(-velocidade, -velocidade);
	else if (direcao == 6) // Tras Esquerda
		kit.setSpeed(-velocidade/3, -velocidade);
	else if (direcao == 7) // Esquerda
		kit.setSpeed(-velocidade, velocidade);
	else if (direcao == 8) // Frente Esquerda
		kit.setSpeed(velocidade/3, velocidade);
}
