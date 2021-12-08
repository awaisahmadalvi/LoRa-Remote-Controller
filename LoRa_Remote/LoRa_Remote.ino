#include "LoRa.h"
#include "debug.h"

#define DEBUG_ENABLE

byte BUILTIN_LED = 13;

byte ANT_BTN = 9;

byte RED_BTN = 10;
byte GRN_BTN = 11;
byte BLU_BTN = 12;

bool IN_ISR = false;

void setup()
{

#ifdef DEBUG_ENABLE
  Serial.begin(9600);
  //while (!Serial) ; // Wait for serial port to be available
#endif

  LoRa_Setup();


  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

  pinMode(ANT_BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ANT_BTN), ANT_BTN_ACT, FALLING );

  pinMode(RED_BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RED_BTN), RED_BTN_ACT, FALLING );

  pinMode(GRN_BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(GRN_BTN), GRN_BTN_ACT, FALLING );

  pinMode(BLU_BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BLU_BTN), BLU_BTN_ACT, FALLING );
}

uint8_t * action;
char trigger[] = "trigger";
uint8_t R__[] = "R++";
uint8_t G__[] = "G++";
uint8_t B__[] = "B++";

void loop()
{
  if (IN_ISR)
  {
    LoRa_Send(action);
    IN_ISR = false;
  }
  delay(20);
}


void ANT_BTN_ACT() {
  if (IN_ISR)
    return;
  IN_ISR = true;
  debugln("ANT_BTN_ACT");
  digitalWrite(BUILTIN_LED, LOW);
  action = (uint8_t *)trigger;
  digitalWrite(BUILTIN_LED, HIGH);
  debugln("ANT_BTN_ACT_EXIT");
}

void RED_BTN_ACT() {
  if (IN_ISR)
    return;
  IN_ISR = true;
  debugln("RED_BTN_ACT");
  digitalWrite(BUILTIN_LED, LOW);
  action = (uint8_t *)R__;
  digitalWrite(BUILTIN_LED, HIGH);
  debugln("RED_BTN_ACT_EXIT");
}
void GRN_BTN_ACT() {
  if (IN_ISR)
    return;
  IN_ISR = true;
  debugln("GRN_BTN_ACT");
  digitalWrite(BUILTIN_LED, LOW);
  action = (uint8_t *)G__;
  digitalWrite(BUILTIN_LED, HIGH);
  debugln("GRN_BTN_ACT_EXIT");
}
void BLU_BTN_ACT() {
  if (IN_ISR)
    return;
  IN_ISR = true;
  debugln("BLU_BTN_ACT");
  digitalWrite(BUILTIN_LED, LOW);
  action = (uint8_t *)B__;
  digitalWrite(BUILTIN_LED, HIGH);
  debugln("BLU_BTN_ACT_EXIT");
}
