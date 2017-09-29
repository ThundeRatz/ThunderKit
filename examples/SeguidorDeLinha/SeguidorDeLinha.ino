#include "ThunderKit.h"


ThunderKit kit(1);
int i, sensores[5];

void setup() {
	if (kit.begin() != 0) {
		while (1) {
			Serial.println("Erro na inicialização :(");
			delay(5000);
		}
	}
}

#define BLACK 0
#define WHITE 1
void loop() {
    // Faz a leitura de todos os sensores e guarda em um vetor
    for (i; i < 5; i++){
        sensores[i] = kit.getColor(i);
    }

    // Caso a linha esteja na esquerda
    if ((sensores[0] == WHITE) && (sensores[4] == BLACK)) {
        // Acelera mais a roda da direita
        kit.setSpeed(10, 50); 
    } 
    // Caso a linha esteja na direita
    else if ((sensores[0] == BLACK) && (sensores[4] == WHITE)) {
        // Acelera mais a roda da esquerda
        kit.setSpeed(50, 10);
    } 
    //Caso não leia a linha
    else {
        // Acelera as duas rodas igualmente
        kit.setSpeed(50, 50);
    }

}
