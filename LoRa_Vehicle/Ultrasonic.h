#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

const int TRIG_PIN = 10; // Trigger Pin of Ultrasonic Sensor
const int ECHO_PIN = 11; // Echo Pin of Ultrasonic Sensor

void US_setup();
long US_Get_Distance();


long microsecondsToInches(long microseconds);
long microsecondsToCentimeters(long microseconds);

#endif
