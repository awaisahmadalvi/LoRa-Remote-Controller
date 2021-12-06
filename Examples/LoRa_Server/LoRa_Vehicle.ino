#include "LoRa.h"
#include "debug.h"
#include <NewPing.h>

#define DEBUG_ENABLE

//Ultrasonic sensor
#define TRIG_PIN 14
#define ECHO_PIN 15
#define MAX_DISTANCE 500
#define ANT_HEIGHT 200
int distance = 0;

#define OPEN_LMT_SW     9
#define CLOSE_LMT_SW    10

#define FOLD_PIN_1      11
#define FOLD_PIN_2      12

#define ANT_PIN_1       11
#define ANT_PIN_2       12

#define RED_PIN         5
#define GRN_PIN         6
#define BLU_PIN         3

byte RED_PWM = 0;
byte GRN_PWM = 0;
byte BLU_PWM = 0;

NewPing sonar = NewPing(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

void setup()
{
#ifdef DEBUG_ENABLE
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
#endif

  LoRa_Setup();
}

void loop()
{
  LoRa_Read();
  //debugln((char*)action);
  delay(1000);
}
