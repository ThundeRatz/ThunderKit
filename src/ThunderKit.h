#ifndef _THUNDERKIT_H_
#define _THUNDERKIT_H_

#ifndef ARDUINO_AVR_LEONARDO
#	error Board type should be Arduino Leonardo
#endif

#include "Arduino.h"
#include<vector.h>

// Motors
#define AIN1   D13
#define AIN2   D11
#define BIN1   D9
#define BIN2   D10
#define nSLEEP D7

#define FAST_ACCEL 0x00
#define SLOW_ACCEL 0x01

#define ESQ 0x00
#define DIR 0x01

#define HOR 0x01
#define AHOR 0x00

// LEDs
#define LED    D13
#define LEDR   SCL
#define LEDG   D5
#define LEDB   D6

class ThunderKit {
	public:
		ThunderKit(int kit_number);

		int begin();
		
		/*
		 * Sensores de linha
		 */
		 
		void addSensor(int pin, int threshold);
		void setThreshold(int num_sensor, int threshold);
		uint8_t getColor(int num_sensor);
		uint16_t getReading(int num_sensor);
		
		/*
		 * Motores
		 */
		 
		void setSpeed(int side, int dir, int speed);
		void stopAll();
		
	private:
	
		/*
		 *	Bluetooth
		 */
		char at_name[20];
		char recv_str[100];
		int recv_msg(int timeout);
		int send_msg(char* msg);
		
		/*
		 *	Sensores de linha
		 */
		struct sensor_linha{
			uint8_t pino;
			uint8_t limiar;
		}
		sensor_linha sensors[6];
};

#endif
