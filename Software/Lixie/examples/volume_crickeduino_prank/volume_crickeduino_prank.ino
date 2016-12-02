/* ---------------------------------
   "Crickeduino" by Connor Nishijima
   ---------------------------------

   Released under the GPLv3 License
   http://www.gnu.org/licenses/gpl-3.0.en.html

   This is a prank sketch designed to produce convincing cricket noises
   at random intervals and go into deep sleep when not in use. Loudness
   attenuation is done using my Volume library:

   https://github.com/connornishijima/arduino-volume

   I developed this library specifically for this project, so that I
   could hide a "cricket" in my pal's vent with magnets. By driving the
   speaker or piezo with a PWM whose frequency is higher than your hearing
   range, you can attenuate the perceived volume of the sound creating a
   more convincing cricket.

   Deep sleep is acheived with the amazing JeeLib "Sleepy" function.
   Using an ATmega328p with nothing but the speaker and a ceramic resonator
   to drive it at 16MHz with 5 volts, current usage is as follows:

   Awake: 16.0 mA
   Sleep:  0.2 mA

   Estimated life on 2x 2500mAh 18650 batteries (6.6V): 438 days

   (You will also lose a bit of current depending on what 5V regulator you
   use, bringing the total days a bit lower. Stick to switching regulators
   as opposed to linears like the LM7805.)

   I haven't gotten my Volume library to run properly at 8MHz / 3.3V,
   but if you can the efficency would increase by a factor of 5:

   Awake:  3.60 mA
   Sleep:  0.04 mA

   Estimated life on 2500mAh 18650 battery: 2,191 days - no regulator needed

   However, running on 3.3 volts results in only 66% the maximum volume,
   so you might have to be creative on how to hide this project effectively.

   16MHz boards only - only tested with ATmega328p / Arduino Uno.
   This will work on 16MHz ATmega32u4 based boards like the Pro Micro if you
   alter the Volume library init to use pin 3 instead of 5 or 6.
*/

#include <Volume.h> // Used for programmatic volume control
#include <JeeLib.h> // Used for deep sleep

Volume vol; // Plug your speaker into the default pin for your board type:
            // https://github.com/connornishijima/arduino-volume#supported-pins
            
ISR(WDT_vect) { // Jeelib Sleepy uses this to wake back up
  Sleepy::watchdogEvent();
}

const bool demoMode = true; // Use this to test your speaker and hear the sounds at 2sec intervals. Set to false for long sleep mode.

const unsigned int sleepTimes[] PROGMEM = {3, 10, 30, 60, 180}; // These are the random sleep intervals: 3min, 10min, 30min, 1hr, 3hr.

unsigned int sleepMinutes = 0;
// This is used to make the cricket do an initial extended
// sleep if you'd like. use anywhere from 0 to 65,535 minutes (45 days!)
// This is good for hiding the payload inconspicuously. ;)

byte repeat = 0; // Used at random to combine noises

void setup() {
  // Do initial sleep if set
  while (sleepMinutes > 0) {
    int sleepTime = 60000;
    Sleepy::loseSomeTime(sleepTime);
    sleepMinutes--;
  }

  // Start timers for Volume lib:
  vol.begin();

  // Take random reading from empty analog pin to seed the Arduino's pseudo-RNG.
  randomSeed(analogRead(A5));
}

void loop() {
  chirpAndSleep(); // Run any extra code above this line
}

void chirpAndSleep() {
  chirp(); // Go decide on a type of noise to make then come back

  int chance = random(1, 10); // 1 in 10 chance of making two or three random noises this cycle
  if (chance == 1) {
    repeat = random(1, 2);
  }

  if (repeat > 0) { // If we're making more than one noise, don't sleep
    repeat--;
  }
  else { // No noises left, time to sleep
    if (demoMode == true) { // If we're in demo mode, don't sleep.
      vol.delay(2000);
    }
    else { // If not, sleep tight!
      int sleepTime = sleepTimes[random(0, 5)] + random(0, 6); // Get random time from sleepTimes list, and add a random 0-5 minutes.
      // Nighty night
      while (sleepTime > 0) { // sleep one minute at a time
        Sleepy::loseSomeTime(60000);
        sleepTime--;
      }
    }
  }
}

void chirp() {
  int freq = random(3700, 4000); // random frequency in this range is picked for this chirp
  float volume = random(10, 100+1) / 100.00; // random volume between 10-100% is chosen for this chirp

  int chirpType = random(0, 3); // randomly pick a type of noise to make

  if (chirpType == 0) { // Burst chirp
    int chirpCount = random(1, 3);
    int count = random(1, 6);
    while (chirpCount > 0) {
      chirpLoud(count, freq, volume);
      vol.delay(100);
      chirpCount--;
    }
  }
  if (chirpType == 1) { // Faded chirp
    int chirpCount = random(1, 3);
    chirpFade(chirpCount, freq, volume);
  }
  if (chirpType == 2) { // Creepy rustle noise
    int chirpCount = random(1, 3);
    int count = 3;
    while (chirpCount > 0) {
      chirpNoise(count, volume);
      vol.delay(10);
      chirpCount--;
    }
  }
}

void chirpLoud(byte times, int freq, float volume) {
  while (times > 0) {
    int v = 0;
    while (v < 255) {
      vol.tone(freq, v * volume);
      v += 4;
    }
    v = 255;
    while (v > 0) {
      vol.tone(freq, v * volume);
      v -= 4;
    }
    vol.delay(10);
    times--;
  }
  vol.noTone();
}

void chirpFade(byte times, int freq, float volume) {
  float fadeOut = 1.00;
  while (times > 0) {
    float mv = 0;
    while (mv < 1.00) {
      int v = 0;
      while (v < 255) {
        vol.tone(freq, v * volume * mv * fadeOut);
        v += 4;
      }
      v = 255;
      while (v > 0) {
        vol.tone(freq, v * volume * mv * fadeOut);
        v -= 4;
      }
      vol.delay(20);
      mv += 0.2;
    }
    mv = 1.00;
    while (mv > 0.00) {
      int v = 0;
      while (v < 255) {
        vol.tone(freq, v * volume * mv * fadeOut);
        v += 4;
      }
      v = 255;
      while (v > 0) {
        vol.tone(freq, v * volume * mv * fadeOut);
        v -= 4;
      }
      vol.delay(20);
      mv -= 0.2;
    }
    vol.noTone();
    fadeOut -= 0.75;
    times--;
  }
}

void chirpNoise(byte times, float volume) {
  while (times > 0) {
    int v = 0;
    while (v < 128) {
      vol.tone(random(6000, 10000), v * volume);
      v += 4;
    }
    v = 128;
    while (v > 0) {
      vol.tone(random(6000, 10000), v * volume);
      v -= 4;
    }
    vol.delay(10);
    times--;
  }
  vol.noTone();
}

// Special thanks to Andrew Neal for dealing with this cricket for what's hopefully months.
