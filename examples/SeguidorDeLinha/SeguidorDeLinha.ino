#include "ThunderKit.h"

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

}
