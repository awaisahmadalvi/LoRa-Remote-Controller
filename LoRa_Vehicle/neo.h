#ifndef NEO_H
#define NEO_H

#include <Adafruit_NeoPixel.h>


// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN     5

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT  5

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 255 // Set BRIGHTNESS to about 1/5 (max = 255)


void neo_setup();
void LED_ON(int red, int green, int blue);
void LED_OFF();
void colorWipe(uint32_t color, int wait);

#endif
