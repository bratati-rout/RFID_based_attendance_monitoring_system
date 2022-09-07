// see here how to use this library: http://www.youtube.com/watch?v=kVw6kPSJJfw


#ifndef ANMAKEYPAD_H
#define ANMAKEYPAD_H

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#define KEY_NOT_PRESSED '-'
#define KEY_1 '1'
#define KEY_2 '2'
#define KEY_3 '3'
#define KEY_A 'A'
#define KEY_4 '4'
#define KEY_5 '5'
#define KEY_6 '6'
#define KEY_B 'B'
#define KEY_7 '7'
#define KEY_8 '8'
#define KEY_9 '9'
#define KEY_C 'C'
#define KEY_STAR '*'
#define KEY_0 '0'
#define KEY_HASH '#'
#define KEY_D 'D'
#define KEY_1_2 'a'
#define KEY_1_3 'b'
#define KEY_1_4 'c'
#define KEY_1_5 'd'
#define KEY_2_3 'e'
#define KEY_2_4 'f'
#define KEY_2_5 'g'
#define KEY_3_4 'h'
#define KEY_3_5 'i'
#define KEY_4_5 'j'

class AnalogMatrixKeypad{
	public:
		AnalogMatrixKeypad(byte analogPin);
		void setDebounceTime(unsigned int debounceTime);
		void setNumberOfKeys(unsigned int numberOfKeys);
		void setThresholdValue(byte tv);
		char readKey();
	private:
		byte analogPin;
		unsigned int debounceTime;
		long lastValue;
		byte threshold;
		unsigned int numberOfKeys;
};
#endif

// see here how to use this library: http://www.youtube.com/watch?v=kVw6kPSJJfw
