#ifndef _THUNDERKIT_H_
#define _THUNDERKIT_H_

#ifndef ARDUINO_AVR_LEONARDO
#	error Board type should be Arduino Leonardo
#endif

#include "Arduino.h"

// Motors
#define AIN1   13
#define AIN2   11
#define BIN1   9
#define BIN2   10
#define nSLEEP 7

// LEDs
#define LED    13
#define LEDR   SCL
#define LEDG   5
#define LEDB   6

#define FIRST_SENSOR A1

class ThunderKit {
	public:
		ThunderKit(int kit_number);

		int begin();

		//LEDs
		void ligar_led(int led);
		void desligar_led(int led);
		void intens_led(int led, int intensidade);


		// Sensores linha
		void addSensor(int pin, int threshold);
		void setThreshold(int num_sensor, int threshold);
		int getColor(int num_sensor);
		int getReading(int num_sensor);

		// Motores
		void setSpeed(int vel_esq, int vel_dir);
		void stopAll();


	private:

		// Bluetooth
		char at_name[20];
		char recv_str[100];
		int recv_msg(int timeout);
		int send_msg(char* msg);

		struct sensor_linha {
			int pino;
			int limiar;

			sensor_linha() : pino(-1), limiar(-1) {}
		} sensors[5];
};

#endif
