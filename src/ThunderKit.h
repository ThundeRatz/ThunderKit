#ifndef _THUNDERKIT_H_
#define _THUNDERKIT_H_

#ifndef ARDUINO_AVR_LEONARDO
#	error Board type should be Arduino Leonardo
#endif

#include "Arduino.h"

// Motors
#define AIN1   13
#define AIN2   10
#define BIN1   11
#define BIN2   9

// LEDs
#define LED    13
#define LEDR   SCL
#define LEDG   5
#define LEDB   6

#define DIRECAO 0
#define VELOCIDADE 1

class ThunderKit {
	public:
		ThunderKit(int kit_number);

		int begin();

		// App Commands
		int appCommand();
		int joystick();
		int joystick(int tipo);
		boolean seguidor();

		// LEDs
		void ledRGB(int r, int g, int b);
		void led(int intensidade = 100);
		void ledArcoIris();

		// Sensores linha
		void setThreshold(int posicao, int threshold);
		int getColor(int posicao);
		int lerSensor(int posicao);

		// Motores
		void ativarMotores();
		void setSpeed(int vel_esq, int vel_dir);
		void desativarMotores();
		void sendReadings();

	private:
		// LED azul é ligado junto com os Motores
		// Variavel para nao mexer no led se os motores
		// estiverem ativados
		boolean motors_on;
		boolean seguidor_on;
		int joystick_pos;

		// Bluetooth
		char at_name[20];
		char recv_str[100];

		uint8_t recv_packet[20];

		int recv_msg(int timeout);
		int send_msg(const String& msg);

		struct sensor_linha {
			uint8_t pino;
			uint16_t limiar;

			sensor_linha() : pino(-1), limiar(-1) {}
		} sensors[5];
};

#endif
