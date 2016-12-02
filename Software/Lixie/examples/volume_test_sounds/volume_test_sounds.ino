// Volume Library Demo Sketch (c) 2016 Connor Nishijima
// Released under the GPLv3 license

#include "Volume.h" // Include the Volume library

Volume vol; // Plug your speaker into the default pin for your board type:
            // https://github.com/connornishijima/arduino-volume#supported-pins 

void setup()
{
  vol.begin(); // After calling this, delay() and delayMicroseconds will no longer work
               // correctly! Instead, use vol.delay() and vol.delayMicroseconds() for
               // the correct timing

  vol.setMasterVolume(1.00); // Self-explanatory enough, right? Try lowering this value if the speaker is too loud! (0.00 - 1.00)
  vol.delay(500);
}

void loop()
{
  gameboy();  // Nostalgia!
  vol.delay(1000);

  wolfWhistle();  // Wee-woo!
  vol.delay(1000);
  R2D2();         // BRRT doo doo doo doo doo doo doo doo DIOooo BRRT!
  vol.delay(1000);
  jetsons();   // Wuuuuub wuuuub wuuub wuub wub wub wub wubwubwubwubwub!
  vol.delay(1000);
}



//------------------------------------------------------------------------------------
// Below are some demo sounds I've made - if you have any questions about
// these email me directly at connornishijima@gmail.com :) Feel free to
// use the R2D2 function for your own Star Wars project!
//------------------------------------------------------------------------------------

void gameboy(){
  vol.tone(1025,255); // pa
  vol.delay(70);
  int v = 255;
  while(v > 0){
    vol.tone(2090,v); // ting!
    vol.delay(3);
    v--;
  }
}

void wolfWhistle() {
  int f = 122; // starting frequency
  int v = 0;   // starting volume
  while (f < 4000) {  // slide up to 4000Hz
    vol.tone(f, v);
    v = 255 * (f / 4000.00);
    f += 25;
    vol.delay(1);
  }
  vol.noTone();
  vol.delay(100); // wait a moment
  f = 122; // starting frequency
  v = 0;   // starting volume
  while (f < 3000) { // slide up to 3000Hz
    vol.tone(f, v);
    v = 255 * (f / 4000.00); 
    f += 25;
    vol.delay(2);
  }
  while (f > 125) { // slide down to 125Hz
    vol.tone(f, v);
    v = 255 * (f / 4000.00);
    f -= 25;
    vol.delay(2);
  }
  vol.noTone(); // end tone production
}

void R2D2() {
  int beeps[] = {1933, 2156, 1863, 1505, 1816, 1933, 1729, 2291};
  int buzzVols[] = {144, 180, 216, 252, 252, 252, 252, 216, 180, 144};

  int i = 9;
  while (i >= 0) {
    vol.tone(1050, buzzVols[i]);
    vol.delayMicroseconds(20);
    vol.tone(1050, buzzVols[i] / 8);
    vol.delayMicroseconds(40);
    i--;
  }

  vol.delay(35);

  i = 0;
  while (i < 8) {
    int v = 0;
    while (v < 250) { // 12.5 mS fade up time
      vol.tone(beeps[i], v);
      v += 10;
      vol.delayMicroseconds(2);
    }
    vol.delay(20);
    v = 250;
    while (v > 0) { // 12.5 mS fade down time
      vol.tone(beeps[i], v);
      v -= 10;
      vol.delayMicroseconds(5);
    }
    vol.noTone();
    vol.delay(35);
    i++;
  }

  int f = 2466;
  while (f < 2825) {
    vol.tone(f, 255);
    f += 3;
    vol.delay(1);
  }
  f = 2825;
  int v = 255;
  while (f > 2000) {
    vol.tone(f, v);
    f -= 6;
    v -= 1;
    vol.delay(1);
  }
  vol.noTone();
  vol.delay(35);

  i = 10;
  while (i > 0) {
    vol.tone(1050, buzzVols[i]);
    vol.delayMicroseconds(20);
    vol.tone(1050, buzzVols[i] / 8);
    vol.delayMicroseconds(40);
    i--;
  }
  vol.noTone();
}

void jetsons() {

  int f = 659;       // MEET
  int v = 254;
  while(v > 0){
    vol.tone(f,v);
    v-=2;
    vol.delay(1);
  }
  vol.delay(250);
  f = 831;           // GEORGE
   v = 254;
  while(v > 0){
    vol.tone(f,v);
    v-=2;
    vol.delay(1);
  }
  vol.delay(250);
  f = 932;           // JET
  v = 254;
  while(v > 0){
    vol.tone(f,v);
    v-=2;
    vol.delay(1);
  }
  vol.delay(50);
  f = 988;           // SON!
  v = 254;
  while(v > 32){
    vol.tone(f,v);
    v-=1;
    vol.delay(2);
  }
  delay(250);
  int fadeV = 1;
  
  while (f < 7000) {
    vol.tone(f / 4, constrain(v,0,255));
    f += 3;

    if (v < 200 && fadeV == 1) {
      v += ((f/7000.00)*100)/10;
    }
    else {
      fadeV = 0;
    }

    if (v > -50 && fadeV == 0) {
      v -= ((f/7000.00)*100)/10;
    }
    else {
      fadeV = 1;
    }

    vol.delay(1);
  }

  int i = 8000;
  float multiplier = 1.00;
  while(i > 0){
    vol.tone(f / 4, constrain(v,0,255)*multiplier);
    
    if (v < 200 && fadeV == 1) {
      v += ((f/8000.00)*100)/10;
    }
    else {
      fadeV = 0;
    }

    if (v > -20 && fadeV == 0) {
      v -= ((f/8000.00)*100)/10;
    }
    else {
      fadeV = 1;
    }

    if(multiplier > 0){
      multiplier -= 0.0002;
    }
    else{
      multiplier = 0;
    }
    if(i % 4 == 0){
      f-=1;
    }
    vol.delay(1);
    i--;
  }
  vol.noTone();
}
