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

#define OPEN_LMT_SW     18
#define CLOSE_LMT_SW    19

#define FOLD_PIN_1      16
#define FOLD_PIN_2      17

#define ANT_PIN_1       14
#define ANT_PIN_2       15

#define RED_PIN         10
#define GRN_PIN         11
#define BLU_PIN         12

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

  pinMode(OPEN_LMT_SW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(OPEN_LMT_SW), stopUnfolding, FALLING );

  pinMode(CLOSE_LMT_SW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLOSE_LMT_SW), stopFolding, FALLING );

  pinMode(FOLD_PIN_1, OUTPUT);
  pinMode(FOLD_PIN_2, OUTPUT);

  pinMode(ANT_PIN_1, OUTPUT);
  pinMode(ANT_PIN_2, OUTPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GRN_PIN, OUTPUT);
  pinMode(BLU_PIN, OUTPUT);
}
uint8_t* action;
void loop()
{
  //action = 0;
  action = LoRa_Read();
  //debug("return: ");
  //debugln((char*)action);
  
  if (strcmp((char*)action, "")==0)
    return;
  if (strcmp((char*)action, "trigger")==0)
  {
    debugln("IN TRIGGER");
    //triggerAntenna();
  }
  else if (strcmp((char*)action, "R++")==0)
  {
    debugln("Red PWM Increment");
    // Increment Red by 64
    RED_PWM = (RED_PWM + 64) % 256;
    analogWrite(RED_PIN, RED_PWM);
  }
  else if (strcmp((char*)action, "G++")==0)
  {
    debugln("Green PWM Increment");
    // Increment Green by 64
    RED_PWM = (GRN_PWM + 64) % 256;
    analogWrite(GRN_PIN, GRN_PWM);
  }
  else if (strcmp((char*)action, "B++")==0)
  {
    debugln("Blue PWM Increment");
    // Increment Blue by 64
    RED_PWM = (BLU_PWM + 64) % 256;
    analogWrite(BLU_PIN, BLU_PWM);
  }
  delay(20);
}

void triggerAntenna()
{
  //byte close = digitalRead(CLOSE_LMT_SW);
  //byte open = digitalRead(OPEN_LMT_SW);
  if (digitalRead(OPEN_LMT_SW) == HIGH)   // Clossing antenna assembly
  {
    closeAnt();
  }
  else            //if(close==HIGH) Opening Antenna Asembly
  {
    /* Check Antenna Clearance */
    if (checkClearance()) // If clear then open antenna
    {
      unfoldAnt();
    }
  }
}

void openAnt()
{
  debugln("Uncoiling Antenna");
  digitalWrite(ANT_PIN_1, HIGH);
  digitalWrite(ANT_PIN_2, LOW);
  delay(5000);
  digitalWrite(ANT_PIN_1, LOW);
  digitalWrite(ANT_PIN_2, LOW);
}

void closeAnt()
{
  debugln("Coiling Antenna");
  digitalWrite(ANT_PIN_1, LOW);
  digitalWrite(ANT_PIN_2, HIGH);
  delay(5000);
  digitalWrite(ANT_PIN_1, LOW);
  digitalWrite(ANT_PIN_2, LOW);
  foldAnt();
}

void unfoldAnt()
{
  debugln("Unfolding Antenna");
  digitalWrite(FOLD_PIN_1, HIGH);
  digitalWrite(FOLD_PIN_2, LOW);
}

void foldAnt()
{
  debugln("Folding Antenna");
  digitalWrite(FOLD_PIN_1, LOW);
  digitalWrite(FOLD_PIN_2, HIGH);
}

void stopFolding()
{
  debugln("Stop Folding Antenna");
  digitalWrite(FOLD_PIN_1, LOW);
  digitalWrite(FOLD_PIN_2, LOW);
}

void stopUnfolding()
{
  debugln("stop Unfolding Antenna");
  digitalWrite(FOLD_PIN_1, LOW);
  digitalWrite(FOLD_PIN_2, LOW);
  openAnt();
}

bool checkClearance()
{
  debugln("Checking Antenna Clearance");
  /* Interface Ultrasonic Sensor & check distance*/
  distance = sonar.ping_cm();
  debug("Height is: ");
  debugln(distance);
  if (distance >= ANT_HEIGHT)
    return true;
  return false;
}
