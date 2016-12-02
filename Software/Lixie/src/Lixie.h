/*
  Lixie.h - Library for driving Lixie displays!
  Created by Connor Nishijima, October 26th 2016.
  Released under the GPLv3 license.
*/

#ifndef lixie_h
#define lixie_h

#include "Arduino.h"
#include "FastLED.h"

#define len( x )  ( sizeof( x ) / sizeof( *x ) )
#define write_char(input) write_char_f(input,len(input)-1)

class Lixie
{
  public:
    Lixie();
    void begin();
	void clear();
	void set_color(byte r, byte g, byte b);
	void set_color(byte r, byte g, byte b, int pos);
	void set_padding(bool state);
	void set_full_lights(bool lights);
	void print_color();
	void print_binary();
	void brightness(byte val);
	void write_char_f(char* input, byte input_length);
	void write_int(uint64_t input);
	void write(byte val, byte pos);
	void update();
};

#endif
