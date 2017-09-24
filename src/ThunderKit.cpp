#include "Arduino.h"
#include "Wire.h"

#include <string.h>

#include "ThunderKit.h"

ThunderKit::ThunderKit(int kit_number) {
	sprintf(at_name, "AT+NAMBThunderKit%d", kit_number);
}

int ThunderKit::begin() {
	Serial.begin(9600);
	Serial.println("ThunderKit started!");

	// Bluetooth setup
	Serial.println("Configurando tudo...");
	Serial.println("-- Bluetooh");
	delay(1000);

	for (;;) {
		delay(500);
		Serial1.begin(9600);
		delay(500);
		Serial.println("try 9600...");
		if(send_msg("AT") == 0) {
			break;
		}
		Serial.println(" NO");

		delay(500);
		Serial1.begin(115200);
		delay(500);
		Serial.println("try 115200...");
		if(send_msg("AT") == 0) {
			break;
		}
		Serial.println(" NO");
	}

	// Reconfigurar Bluetooth
	send_msg("AT+RENEW");
	send_msg("AT+BAUD2");
	send_msg("AT+AUTH0");
	send_msg("AT+RESET");
	Serial1.begin(9500);
	delay(2000);

	send_msg("AT+VERS?");
	send_msg("AT+ADDB?");
	send_msg(at_name);
	send_msg("AT+SCAN0");
	send_msg("AT+NOTI1");
	send_msg("AT+NOTP0");
	send_msg("AT+ROLB0");

	send_msg("AT+RESET");
	delay(2000);

	if (send_msg("AT") != 0)
		return -1;

	Serial.println("-- Bluetooth OK");
	Serial.println();

	// Confugracao motores e outras coisas
	for(int i = 0; i < 5; i++){		
		sensors[i].pino = -1; sensors[i].limiar = -1;
	}
	Serial.println("configuração concluida!");

	return 0;
}

int ThunderKit::recv_msg(int timeout) {
	uint16_t time = millis();
	uint8_t i = 0;

	for (;;) {
		if (Serial1.available()) {
			recv_str[i++] = char(Serial1.read());
			break;
		}
		if (millis() - time > timeout)
			return -1;
	}

	delay(30);

	while (Serial1.available() && i < 100) {
		recv_str[i++] = char(Serial1.read());
	}

	recv_str[i] = '\0';

	return 0;
}

int ThunderKit::send_msg(char* msg) {
	Serial.print("Enviando: ");
	Serial.println(msg);

	Serial1.print(msg);
	delay(1);

	if(recv_msg(500) != 0)
		return -1;

	Serial.print("Recebido: ");
	Serial.println(recv_str);

	return 0;
}

#define FIRST_SENSOR A1

void ThunderKit::addSensor(int pin, int threshold=512){
	
	/*
		Recebe: Numero do pino do sensor e limiar de cor. Valor default para limiar = 512
		Reserva um espaco na memoria para o sensor
	*/
	
	sensors[pin-FIRST_SENSOR].pino = pin;  sensors[pin-FIRST_SENSOR].limiar = threshold;
	pinMode(pin, INPUT);
}

void ThunderKit::setThreshold(int pin, int threshold){
	
	/*
		Recebe: Numero do sensor| limiar a ser designado
	*/
	
	sensors[pin-FIRST_SENSOR].limiar = threshold;	
}

uint8_t ThunderKit::getColor(int num_sensor){
	
	/*
		Recebe: Numero do sensor
		Retorna: 0 - caso abaixo do limiar (preto) | 1 - caso acima do limiar (branco) | -1 - caso num_sensor invalido
	*/
	
	uint16_t reading = analogRead(sensors[num_sensor-FIRST_SENSOR].pino);
	return reading >= sensors[num_sensor-FIRST_SENSOR].limiar;
}

uint16_t ThunderKit::getReading(int num_sensor){
	
	/*
		Recebe: Numero do sensor
		Retorna: Leitura analogica do pino
	*/

  if(num_sensor > 5 || num_sensor < 0){
    return -1;  
  }
	if(sensors[num_sensor-FIRST_SENSOR].pino == -1){
		return -1;
	}
	uint16_t reading = analogRead(sensors[num_sensor-FIRST_SENSOR].pino);
	return reading;
}

void ThunderKit::setSpeed(int vel_esq, int vel_dir){
	
	/*
		Recebe: vel_esq (-100 -> 100) | vel_dir (-100 -> 100)
		vel_esq	-> Velocidade do motor esquerdi
		vel_dir	-> Qual o sentido de rotacao
		
		De acordo com a pg 9 do datasheet http://www.ti.com/lit/ds/symlink/drv8833.pdf
	 */
	vel_esq = constrain(vel_esq, -100, 100);
	vel_dir = constrain(vel_dir, -100, 100);
	
	if(vel_esq > 0){
		vel_esq = map(vel_esq, 0, 100, 0, 255);
		analogWrite(AIN1, vel_esq);
		analogWrite(AIN2, 0);
	}else{
		vel_esq = map(vel_esq, 0, -100, 0, 255);
		analogWrite(AIN1, 0);
		analogWrite(AIN2, vel_esq);
	}
	
	if(vel_dir > 0){
		vel_esq = map(vel_dir, 0, 100, 0, 255);
		analogWrite(BIN2, vel_dir);
		analogWrite(BIN1, 0);
	}else{
		vel_esq = map(vel_dir, 0, -100, 0, 255);
		analogWrite(BIN2, 0);
		analogWrite(BIN1, vel_dir);
	}
}
	
void ThunderKit::stopAll(){
	/*
		Para os dois motores rapidamente
	 */
	
	analogWrite(AIN1, 0);
	analogWrite(BIN1, 0);
	analogWrite(AIN2, 0);
	analogWrite(BIN2, 0);
}
