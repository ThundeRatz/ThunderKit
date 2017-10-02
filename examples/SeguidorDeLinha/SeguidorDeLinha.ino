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
    if ((kit.getColor(0) == WHITE) && (kit.getColor(4) == BLACK)) {
        // Acelera mais a roda da direita
        kit.setSpeed(10, 50);
    }
    // Caso a linha esteja na direita
    else if ((kit.getColor(0) == BLACK) && (kit.getColor(4) == WHITE)) {
        // Acelera mais a roda da esquerda
        kit.setSpeed(50, 10);
    }
    //Caso não leia a linha
    else {
        // Acelera as duas rodas igualmente
        kit.setSpeed(50, 50);
    }

}
