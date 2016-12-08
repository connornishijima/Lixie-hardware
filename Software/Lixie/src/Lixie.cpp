/*
  Lixie.cpp - Library for driving Lixie displays!
  Created by Connor Nishijima, October 26th 2016.
  Released under the GPLv3 license.
*/

#include "Lixie.h"
#include "config.h"

#define NUM_LEDS NUM_DIGITS * 20
CRGB leds[NUM_LEDS];

byte led_states[NUM_LEDS];
byte addresses[10] = {3, 4, 2, 0, 8, 6, 5, 7, 9, 1};
byte colors[NUM_DIGITS][3];
byte colors_off[NUM_DIGITS][3];

Lixie::Lixie()
{
}

void hsvToRgb(double h, double s, double v, byte rgb[]) {
    double r2, g2, b2;

    h/=255.0;
    s/=255.0;
    v/=255.0;

    int i = int(h * 6);
    double f = h * 6 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    switch(i % 6){
        case 0: r2 = v, g2 = t, b2 = p; break;
        case 1: r2 = q, g2 = v, b2 = p; break;
        case 2: r2 = p, g2 = v, b2 = t; break;
        case 3: r2 = p, g2 = q, b2 = v; break;
        case 4: r2 = t, g2 = p, b2 = v; break;
        case 5: r2 = v, g2 = p, b2 = q; break;
    }

    rgb[0] = r2 * 255;
    rgb[1] = g2 * 255;
    rgb[2] = b2 * 255;
}

void Lixie::begin() {
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.show();
  for(byte i = 0; i < NUM_DIGITS; i++){
	for(byte c = 0; c < 3; c++){
	  colors[i][c] = 255;
	  colors_off[i][c] = 0;
	}
  }
  clear();
}

void Lixie::clear() {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    led_states[i] = 0;
  }
}

void Lixie::show(){
  for(uint16_t i = 0; i < NUM_LEDS; i++){
    if(led_states[i] == 1){
	  byte r = colors[i/20][0];
	  byte g = colors[i/20][1];
	  byte b = colors[i/20][2];
      leds[i] = CRGB(r,g,b);
    }
    else{
      byte r = colors_off[i/20][0];
	  byte g = colors_off[i/20][1];
	  byte b = colors_off[i/20][2];
      leds[i] = CRGB(r,g,b);
    }
  }
  FastLED.show();
}

// set all on color ------------------------------------
void Lixie::color_on_rgb(byte r, byte g, byte b){
  for(byte i = 0; i < NUM_DIGITS; i++){
	colors[i][0] = r;
	colors[i][1] = g;
	colors[i][2] = b;
  }
}

void Lixie::color_on_hsv(byte h, byte s, byte v){
  for(byte i = 0; i < NUM_DIGITS; i++){
	byte rgb[3];
	hsvToRgb(h,s,v,rgb);
	colors[i][0] = rgb[0];
	colors[i][1] = rgb[1];
	colors[i][2] = rgb[2];
  }
}

// set index on color ------------------------------------
void Lixie::color_on_rgb(byte r, byte g, byte b, byte index){
  colors[index][0] = r;
  colors[index][1] = g;
  colors[index][2] = b;
}

void Lixie::color_on_hsv(byte h, byte s, byte v, byte index){
  byte rgb[3];
  hsvToRgb(h,s,v,rgb);
  colors[index][0] = rgb[0];
  colors[index][1] = rgb[1];
  colors[index][2] = rgb[2];
}

// set all off color -------------------------------------
void Lixie::color_off_rgb(byte r, byte g, byte b){
  for(byte i = 0; i < NUM_DIGITS; i++){
	colors_off[i][0] = r;
	colors_off[i][1] = g;
	colors_off[i][2] = b;
  }
}

void Lixie::color_off_hsv(byte h, byte s, byte v){
  for(byte i = 0; i < NUM_DIGITS; i++){
    byte rgb[3];
	hsvToRgb(h,s,v,rgb);
	colors_off[i][0] = rgb[0];
	colors_off[i][1] = rgb[1];
	colors_off[i][2] = rgb[2];
  }
}

// set index color off -----------------------------------
void Lixie::color_off_rgb(byte r, byte g, byte b, byte index){
  colors_off[index][0] = r;
  colors_off[index][1] = g;
  colors_off[index][2] = b;
}

void Lixie::color_off_hsv(byte h, byte s, byte v, byte index){
  byte rgb[3];
  hsvToRgb(h,s,v,rgb);
  colors_off[index][0] = rgb[0];
  colors_off[index][1] = rgb[1];
  colors_off[index][2] = rgb[2];
}

byte get_size(uint16_t input){
  byte places = 0;
  uint16_t multiply = 10;
  bool done = false;

  if(input >= 0){
    places++;
  }

  while(done == false){
    if(input >= multiply){
      places++;
      multiply *= 10;
    }
    else{
      done = true;
    }
  }
 
  return places;
}

void Lixie::write_int(float input){
  clear();

  byte places = get_size(input);
  
  for(byte i = 0; i < places; i++){
    while (input >= 10) {
      input = input / 10.0;
    }
	
    uint16_t input_ones = uint16_t(input);
    push_digit(input_ones);
    float input_next = float(input) - input_ones;
    input = (input_next*10.0)+0.0005;
  }

  show();
}

void Lixie::write_digit(byte input, byte index){
  uint16_t start = (index*20);

  for(uint16_t i = start; i < start+20; i++){
	led_states[i] = 0;
  }
  
  uint16_t L1 = start+addresses[input];
  uint16_t L2 = start+addresses[input] + 10;

  led_states[L1] = 1;
  led_states[L2] = 1;
  
  show();
}

void Lixie::push_digit(byte number) {
  
  if (NUM_DIGITS > 1) {
    for (uint16_t i = NUM_LEDS - 1; i >= 20; i--) {
      led_states[i] = led_states[i - 20];
    }
    for (uint16_t i = 0; i < 20; i++) {
      led_states[i] = 0;
    }
  }
  else {
    clear();
  }

  uint16_t L1 = addresses[number];
  uint16_t L2 = addresses[number] + 10;

  led_states[L1] = 1;
  led_states[L2] = 1;
  led_states[L2] = 1;
  led_states[L2] = 1;
}

bool char_is_number(char input){
  if(input == '0'){
	return true;
  }
  else if(input == '1'){
	return true;
  }
  else if(input == '2'){
	return true;
  }
  else if(input == '3'){
	return true;
  }
  else if(input == '4'){
	return true;
  }
  else if(input == '5'){
	return true;
  }
  else if(input == '6'){
	return true;
  }
  else if(input == '7'){
	return true;
  }
  else if(input == '8'){
	return true;
  }
  else if(input == '9'){
	return true;
  }
  else{
	return false;
  }
}

byte char_to_number(char input){
	return byte(input-48); // convert ascii index to real number
}

void Lixie::write_string_f(char* input, byte len){
  for(byte i = 0; i < len; i++){
	if(char_is_number(input[i]) == true){
	  push_digit(char_to_number(input[i]));
	}
  }
  show();
}

void Lixie::write_char(char input, byte index){
  if(char_is_number(input) == true){
	write_digit(char_to_number(input),index);
  }
}

void Lixie::print_binary() {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    Serial.print(led_states[i]);
    if ((i + 1) % 20 == 0 && i != 0) {
      Serial.print('\t');
    }
  }
  Serial.println();
}