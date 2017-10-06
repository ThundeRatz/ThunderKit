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
#define LEDR   6
#define LEDG   SCL
#define LEDB   5

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
		void enviarSensores();

		// LEDs
		void ledVerde(int intensidade);
		void ledAzul(int intensidade);
		void ledFade();

		// Sensores linha
		void definirLimiar(int posicao, int limiar);
		boolean branco(int posicao);
		int lerSensor(int posicao);

		// Motores
		void motores(int vel_esq, int vel_dir);

	private:
		// LED azul é ligado junto com os Motores
		// Variavel para nao mexer no led se os motores
		// estiverem ativados
		boolean seguidor_on;
		int joystick_pos;

		// Bluetooth
		char at_name[20];
		char recv_str[100];

		uint8_t recv_packet[20];

		int kind;		
		
		int recv_msg(int timeout);
		int send_msg(const String& msg);

		struct sensor_linha {
			uint8_t pino;
			uint16_t limiar;

			sensor_linha() : pino(-1), limiar(-1) {}
		} sensors[5];
};

#endif
