#include "ThunderKit.h"

#define BLACK 0
#define WHITE 1

ThunderKit kit(1);

void setup() {
	if (kit.begin() != 0) {
		while (1) {
			Serial.println("Erro na inicialização :(");
			delay(5000);
		}
	}
}

void loop() {

    // Caso a linha esteja na esquerda
    if ((kit.Branco(0)) && (!kit.Branco(4))) {
        // Acelera mais a roda da direita
        kit.Motores(10, 50);
    }
    // Caso a linha esteja na direita
    else if ((!kit.Branco(0)) && (kit.Branco(4))) {
        // Acelera mais a roda da esquerda
        kit.Motores(50, 10);
    }
    //Caso não leia a linha
    else {
        // Acelera as duas rodas igualmente
        kit.Motores(50, 50);
    }

}
