/*
  Lixie.h - Library for driving Lixie displays!
  Created by Connor Nishijima, October 26th 2016.
  Released under the GPLv3 license.
*/

#ifndef lixie_h
#define lixie_h

#include "Arduino.h"
#include "FastLED.h"

#define write_string(input) write_string_f(input, sizeof(input)-1);

class Lixie
{
  public:
    Lixie();
    void begin();
	void clear();

	void write_int(float input);
	void push_digit(byte number);
	void write_digit(byte input, byte index);

	void write_string_f(char* input, byte len);
	void write_char(char input, byte index);

	void show();
	void print_binary();
	
	void color_on_rgb(byte r, byte g, byte b);
	void color_on_hsv(byte h, byte s, byte v);
	
	void color_on_rgb(byte r, byte g, byte b, byte index);
	void color_on_hsv(byte h, byte s, byte v, byte index);
	
	void color_off_rgb(byte r, byte g, byte b);
	void color_off_hsv(byte h, byte s, byte v);
	
	void color_off_rgb(byte r, byte g, byte b, byte index);
	void color_off_hsv(byte h, byte s, byte v, byte index);
};

#endif
