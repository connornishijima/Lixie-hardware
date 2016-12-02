/*
  Lixie.cpp - Library for driving Lixie displays!
  Created by Connor Nishijima, October 26th 2016.
  Released under the GPLv3 license.
*/

#include "Lixie.h"
#include "config.h"

#define NUM_LEDS NUM_DIGITS * 20
CRGB leds[NUM_LEDS];

const byte digit_arrangement[10] = {4, 5, 3, 1, 9, 7, 6, 8, 0, 2};
byte number_buf[NUM_DIGITS] = {};
byte colors[NUM_DIGITS*3] = {};
byte master_brightness = 100;
byte led_states[NUM_DIGITS*20] = {};
bool zero_padding = false;
bool full_lights = true;

Lixie::Lixie()
{
}

void Lixie::begin() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  for (uint16_t i = 0; i < NUM_DIGITS*3; i++) {
    colors[i] = 255;
  }
  clear();
}

void Lixie::clear() {
  for (uint16_t i = 0; i < NUM_DIGITS; i++) {
    number_buf[i] = 0;
  }
  
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    led_states[i] = 0;
  }
}

void Lixie::set_padding(bool state) {
  zero_padding = state;
}

void Lixie::set_full_lights(bool lights) {
  full_lights = lights;
}

void Lixie::set_color(byte r, byte g, byte b){
  for (uint16_t i = 0; i < NUM_DIGITS; i++) {
    colors[(3*i)+0] = r;
    colors[(3*i)+1] = g;
    colors[(3*i)+2] = b;
  }
}

void Lixie::set_color(byte r, byte g, byte b, int pos){
  colors[(3*pos)+0] = r;
  colors[(3*pos)+1] = g;
  colors[(3*pos)+2] = b;
}

void Lixie::print_color(){
  for (uint16_t i = 0; i < NUM_DIGITS*3; i++) {
    Serial.print(colors[i]);
	Serial.print(" ");
  }
  Serial.println();
}

void Lixie::print_binary(){
  for(uint16_t i = 0; i < NUM_LEDS; i++){
	if(i%20==0 && i != 0){
	  Serial.print(" ");
	}
	Serial.print(led_states[i]);
  }
  Serial.println();
}

void Lixie::brightness(byte val){
  master_brightness = val;
}

void Lixie::write_char_f(char* input, byte input_length) {
  clear();
  
  if(zero_padding == true){
	uint16_t enable = 4;
	while(enable < NUM_LEDS){
	  if(enable > (input_length*20)){
		led_states[enable-1] = 1;
	  }
	  enable+=10;
	}
  }

  for (uint16_t i = 0; i < input_length; i++) {
    byte pos = input_length - i - 1;
    byte val = input[i] - '0'; // This converts an ASCII char to a byte ('6' --> 6)
    if (pos < NUM_DIGITS) {
      number_buf[len(number_buf)-pos-1] = val;
      byte L1 = digit_arrangement[val];
      byte L2 = digit_arrangement[val] + 10;

      led_states[L1 + (20 * pos) - 1] = 1;
	  if(full_lights == true){
		led_states[L2 + (20 * pos) - 1] = 1;
	  }
    }
  }
  
  update();
}

void Lixie::update(){
  for(uint16_t i = 0; i < NUM_LEDS; i++){
    byte data_pos = 3*byte(i/20);
    if(led_states[i] == 1){
      leds[i] = CRGB(colors[data_pos+0]*(master_brightness/100.0),colors[data_pos+1]*(master_brightness/100.0),colors[data_pos+2]*(master_brightness/100.0));
    }
    else if(led_states[i] == 0){
      leds[i] = CRGB(0,0,0);
    }
  }
 
  FastLED.show();
}

byte num_len(uint64_t input){
  byte len = 0;
  if(input > 0){
    len=1;
  }
  if(input > 9){
    len=2;
  }
  if(input > 99){
    len=3;
  }
  if(input > 999){
    len=4;
  }
  if(input > 9999){
    len=5;
  }
  if(input > 99999){
    len=6;
  }
  if(input > 999999){
    len=7;
  }
  if(input > 9999999){
    len=8;
  }
  if(input > 99999999){
    len=9;
  }
  if(input > 999999999){
    len=10;
  }

  return len;
}

void Lixie::write_int(uint64_t input){
  uint16_t len = num_len(input)+1;
  char tempBuf[len];
  sprintf(tempBuf, "%lu", input);
  write_char(tempBuf);
}

void Lixie::write(byte val, byte pos){
  number_buf[len(number_buf)-pos-1] = val;
  unsigned char data[NUM_DIGITS+1] = {};
  
  for(byte i = 0; i < len(number_buf); i++){
	data[i] = char(number_buf[i]+48);
  }
  write_char(data);
}