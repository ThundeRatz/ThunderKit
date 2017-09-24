#include "ThunderKit.h"

/*
 * Exemplo de uso do ThunderKit com motores DC
 */
 
 
// Declara o kit, necessario passar o numero 
// para nomear o dispositivo Bluetooth
  
  
ThunderKit kit(1);

void setup(){
	
	// Inicia a biblioteca do kit, aborta a execucao de falhar
	if (kit.begin() != 0) {
		Serial.println("Erro na inicialização :(");
		while(1);
	}
	
}

void loop(){
	
	// Demonstracao do funcionamento dos motores
	
	// Acelera lentamente o motor esquerdo 
	// no sentido horario, com 80% de velocidade
	kit.setSpeed(ESQ, HOR, 80, SLOW_ACCEL);
	
	// Acelera lentamente o motor direito
	// no sentido antihorario, com 80% de velocidade
	kit.setSpeed(DIR, AHOR, 80, SLOW_ACCEL);
	
	delay(2000);
	
	// Desacelera lentamente o motor esquerdo 
	// no sentido horario, ate velocidade zero
	kit.setSpeed(ESQ, HOR, 0, SLOW_ACCEL);
	
	// Desacelera lentamente o motor direito 
	// no sentido antihorario, ate velocidade zero
	kit.setSpeed(DIR, AHOR, 0, SLOW_ACCEL);
	
	delay(1000);
	
	// Acelera rapidamente o motor esquerdo 
	// no sentido antihorario, com 50% de velocidade
	kit.setSpeed(ESQ, AHOR, 50, FAST_ACCEL);
	
	// Acelera rapidamente o motor direito 
	// no sentido horario, com 50% de velocidade
	kit.setSpeed(DIR, HOR, 50, FAST_ACCEL);
	
	delay(2000);
	
	// Para os dois motores rapidamente
	kit.stopAll();
	
	delay(10000);
}
