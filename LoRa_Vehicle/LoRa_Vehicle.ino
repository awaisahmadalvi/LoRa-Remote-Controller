#include "LoRa.h"
#include "debug.h"
#include "Ultrasonic.h"

#define DEBUG_ENABLE

//Ultrasonic sensor
#define ANT_HEIGHT 100
int distance = 0;

#define OPEN_LMT_SW     18
#define CLOSE_LMT_SW    19

#define FOLD_MTR_1      16
#define FOLD_MTR_2      17

#define ANT_MTR_1       14
#define ANT_MTR_2       15

#define RED_PIN         5
#define GRN_PIN         6
#define BLU_PIN         9

byte RED_PWM = 0;
byte GRN_PWM = 0;
byte BLU_PWM = 0;

void setup()
{
#ifdef DEBUG_ENABLE
  Serial.begin(115200);
  //while (!Serial) ; // Wait for serial port to be available
#endif

  LoRa_Setup();
  US_setup();

  pinMode(OPEN_LMT_SW, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(OPEN_LMT_SW), stopUnfolding, FALLING );

  pinMode(CLOSE_LMT_SW, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(CLOSE_LMT_SW), stopFolding, FALLING );

  pinMode(FOLD_MTR_1, OUTPUT);
  pinMode(FOLD_MTR_2, OUTPUT);

  pinMode(ANT_MTR_1, OUTPUT);
  pinMode(ANT_MTR_2, OUTPUT);

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

  if (strcmp((char*)action, "") == 0)
    return;
  if (strcmp((char*)action, "trigger") == 0)
  {
    debugln("IN TRIGGER");
    triggerAntenna();
  }
  else if (strcmp((char*)action, "R++") == 0)
  {
    debugln("Red PWM Increment");
    // Increment Red by 64
    RED_PWM = (RED_PWM + 64) % 256;
  }
  else if (strcmp((char*)action, "G++") == 0)
  {
    debugln("Green PWM Increment");
    // Increment Green by 64
    RED_PWM = (GRN_PWM + 64) % 256;
  }
  else if (strcmp((char*)action, "B++") == 0)
  {
    debugln("Blue PWM Increment");
    // Increment Blue by 64
    RED_PWM = (BLU_PWM + 64) % 256;
  }
  delay(20);
}

void triggerAntenna()
{
  //byte close = digitalRead(CLOSE_LMT_SW);
  //byte open = digitalRead(OPEN_LMT_SW);
  if (digitalRead(OPEN_LMT_SW) == LOW)   // Clossing antenna assembly
  {
    RGB_OFF();
    closeAnt();
    foldAnt();
  }
  else            //if(close==HIGH) Opening Antenna Asembly
  {
    /* Check Antenna Clearance */
    if (checkClearance()) // If clear then open antenna
    {
      unfoldAnt();
      RGB_ON();
      openAnt();
    }
  }
}

void RGB_ON()
{
  analogWrite(RED_PIN, RED_PWM);
  analogWrite(GRN_PIN, GRN_PWM);
  analogWrite(BLU_PIN, BLU_PWM);
}

void RGB_OFF()
{
  analogWrite(RED_PIN, 0);
  analogWrite(GRN_PIN, 0);
  analogWrite(BLU_PIN, 0);
}

void openAnt()
{
  debugln("Uncoiling Antenna");
  digitalWrite(ANT_MTR_1, HIGH);
  digitalWrite(ANT_MTR_2, LOW);
  delay(5000);
  digitalWrite(ANT_MTR_1, LOW);
  digitalWrite(ANT_MTR_2, LOW);
}

void closeAnt()
{
  debugln("Coiling Antenna");
  digitalWrite(ANT_MTR_1, LOW);
  digitalWrite(ANT_MTR_2, HIGH);
  delay(5000);
  digitalWrite(ANT_MTR_1, LOW);
  digitalWrite(ANT_MTR_2, LOW);
}

void unfoldAnt()
{
  debugln("Unfolding Antenna");
  digitalWrite(FOLD_MTR_1, HIGH);
  digitalWrite(FOLD_MTR_2, LOW);
  while (digitalRead(OPEN_LMT_SW) == HIGH);
  stopUnfolding();
}

void foldAnt()
{
  debugln("Folding Antenna");
  digitalWrite(FOLD_MTR_1, LOW);
  digitalWrite(FOLD_MTR_2, HIGH);
  while (digitalRead(CLOSE_LMT_SW) == HIGH);
  stopFolding();
}

void stopFolding()
{
  debugln("Stop Folding Antenna");
  digitalWrite(FOLD_MTR_1, LOW);
  digitalWrite(FOLD_MTR_2, LOW);
}

void stopUnfolding()
{
  debugln("stop Unfolding Antenna");
  digitalWrite(FOLD_MTR_1, LOW);
  digitalWrite(FOLD_MTR_2, LOW);
}

bool checkClearance()
{
  //return true;
  debugln("Checking Antenna Clearance");
  /* Interface Ultrasonic Sensor & check distance*/
  distance = US_Get_Distance();
  debug("Height is: ");
  debugln(distance);
  if (distance >= ANT_HEIGHT)
    return true;
  return false;
}
