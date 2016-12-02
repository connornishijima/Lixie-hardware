// Volume Library Interactive Sketch (c) 2016 Connor Nishijima
// Released under the GPLv3 license

#include "Volume.h" // Include the Volume library

Volume vol; // Plug your speaker into the default pin for your board type:
            // https://github.com/connornishijima/arduino-volume#supported-pins
            
// Warble variables!
int f = 0;
int fade = 1;

int p = 0;
int fadeP = 1;

int v = 0;
int fadeV = 0;

// the setup routine runs once when you press reset:
void setup() {
  vol.begin();
}

// the loop routine runs over and over again forever:
void loop() {
  vol.tone(f + p, v);

  float mv = constrain(analogRead(A0) / 10, 0, 100) / 100.00;
  // 0 Volts   =   0% volume
  // 2.5 Volts =  50% volume
  // 5 Volts   = 100% volume!
  // 
  // Use a potentiometer tied to A0 as a master volume control.
  
  vol.setMasterVolume(mv);
  vol.delay(1);        // delay in between reads for stability

  if (f < 800 and fade == 1) {
    f += 1;
  }
  else {
    fade = 0;
  }

  if (f > 0 and fade == 0) {
    f -= 1;
  }
  else {
    fade = 1;
  }

  if (p < 200 and fadeP == 1) {
    p += 10;
  }
  else {
    fadeP = 0;
  }

  if (p > 0 and fadeP == 0) {
    p -= 10;
  }
  else {
    fadeP = 1;
  }

  if (v < 250 and fadeV == 1) {
    v += 5;
  }
  else {
    fadeV = 0;
  }

  if (v > 65 and fadeV == 0) {
    v -= 5;
  }
  else {
    fadeV = 1;
  }
}
