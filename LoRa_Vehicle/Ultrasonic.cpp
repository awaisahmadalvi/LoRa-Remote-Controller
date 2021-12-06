#include "Ultrasonic.h"

void US_setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

long US_Get_Distance() {
  long duration, inches, cm = 0;
  for (int i = 0 ; i < 10 ; i++) {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIG_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH);
    //inches = microsecondsToInches(duration);
    cm = cm + microsecondsToCentimeters(duration);
  }
  cm /= 10;
  //Serial.print(inches);
  //Serial.print("in, ");
  //debug(cm);
  //debugln("cm");
  return cm;
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
