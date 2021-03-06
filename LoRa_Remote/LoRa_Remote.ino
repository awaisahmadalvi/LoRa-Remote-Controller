#include "LoRa.h"
#include "debug.h"

#define DEBUG_ENABLE

#define VBATPIN A7    // 9

byte CLOSE_ANT_BTN = 5;
byte OPEN_ANT_BTN = 6;  //check available pin

byte RED_BTN = 10;
byte GRN_BTN = 11;
byte BLU_BTN = 12;

byte BUILTIN_LED = 13;

byte OPEN_ANT_LED = 14;
byte CLOSE_ANT_LED = 15;

byte REMOTE_BAT_LED = 18;
byte VEHICLE_BAT_LED = 19;

bool IN_ISR = false;
bool getRemBattery = false;

uint8_t * actSend, * actRecv;
char OPEN_ACT[] = "ANT_OPEN";
char CLOSE_ACT[] = "ANT_CLOS";
uint8_t R__[] = "R++";
uint8_t G__[] = "G++";
uint8_t B__[] = "B++";

unsigned long prevMillis, prevMilBat;

void setup()
{
#ifdef DEBUG_ENABLE
  Serial.begin(115200);
  //while (!Serial) ; // Wait for serial port to be available
#endif

  LoRa_Setup();

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

  pinMode(OPEN_ANT_LED, OUTPUT);
  pinMode(CLOSE_ANT_LED, OUTPUT);
  digitalWrite(OPEN_ANT_LED, LOW);
  digitalWrite(CLOSE_ANT_LED, LOW);

  pinMode(OPEN_ANT_BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(OPEN_ANT_BTN), ANT_Open_Send, FALLING );

  pinMode(CLOSE_ANT_BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLOSE_ANT_BTN), ANT_Close_Send, FALLING );

  pinMode(RED_BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RED_BTN), RED_Btn_Send, FALLING );

  pinMode(GRN_BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(GRN_BTN), GRN_Btn_Send, FALLING );

  pinMode(BLU_BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BLU_BTN), BLU_Btn_Send, FALLING );
}

void loop()
{
  if (IN_ISR)
  {
    LoRa_Send(actSend);
    dispBattery();
    IN_ISR = false;
  }

  Check_Action();

  if (prevMillis != 0)
    if (millis() - prevMillis >= 10000)
    {
      debugln("powering off Antenna LEDs");
      prevMillis = 0;
      digitalWrite(OPEN_ANT_LED, LOW);
      digitalWrite(CLOSE_ANT_LED, LOW);
    }
  if (prevMilBat != 0)
    if (millis() - prevMilBat >= 5000)
    {
      debugln("powering off Battery LEDs");
      prevMilBat  = 0;
      digitalWrite(VEHICLE_BAT_LED, LOW);
      digitalWrite(REMOTE_BAT_LED, LOW);
    }

  delay(20);
}

void Check_Action()
{
  actRecv = 0;
  actRecv = LoRa_Read();
  //debug("return: ");
  //debugln((char*)actRecv);

  if (strcmp((char*)actRecv , "") == 0)
    return;
  else if (strcmp((char*)actRecv , "OPN_ACK1") == 0)
  {
    debugln("LoRa RECV: OPEN_ACK");
    Set_Open_LED();
    getRemBattery = true;
  }
  else if (strcmp((char*)actRecv , "OPN_ACK0") == 0)
  {
    debugln("LoRa RECV: OPEN_ACK");
    Set_Open_LED();
    getRemBattery = false;
  }
  else if (strcmp((char*)actRecv , "CLS_ACK1") == 0)
  {
    debugln("LoRa RECV: CLOS_ACK");
    Set_Close_LED();
    getRemBattery = true;
  }
  else if (strcmp((char*)actRecv , "CLS_ACK0") == 0)
  {
    debugln("LoRa RECV: CLOS_ACK");
    Set_Close_LED();
    getRemBattery = false;
  }
  if (getRemBattery)
    dispBattery();
}

void Set_Open_LED()
{
  debugln("OPEN LED: ON");
  prevMillis = millis();
  digitalWrite(OPEN_ANT_LED, HIGH);
}

void Set_Close_LED()
{
  debugln("CLOSE LED: ON");
  prevMillis = millis();
  digitalWrite(CLOSE_ANT_LED, HIGH);
}

void ANT_Open_Send()
{
  if (IN_ISR)
    return;
  IN_ISR = true;
  debugln("ANT_Open_Send");
  digitalWrite(BUILTIN_LED, LOW);
  actSend = (uint8_t *)OPEN_ACT;
  digitalWrite(BUILTIN_LED, HIGH);
  debugln("ANT_Open_Send_EXIT");
}

void ANT_Close_Send()
{
  if (IN_ISR)
    return;
  IN_ISR = true;
  debugln("ANT_Close_Send");
  digitalWrite(BUILTIN_LED, LOW);
  actSend = (uint8_t *)CLOSE_ACT;
  digitalWrite(BUILTIN_LED, HIGH);
  debugln("ANT_Close_Send_EXIT");
}

void RED_Btn_Send()
{
  if (IN_ISR)
    return;
  IN_ISR = true;
  debugln("RED_Btn_Send");
  digitalWrite(BUILTIN_LED, LOW);
  actSend = (uint8_t *)R__;
  digitalWrite(BUILTIN_LED, HIGH);
  debugln("RED_Btn_Send_EXIT");
}

void GRN_Btn_Send()
{
  if (IN_ISR)
    return;
  IN_ISR = true;
  debugln("GRN_Btn_Send");
  digitalWrite(BUILTIN_LED, LOW);
  actSend = (uint8_t *)G__;
  digitalWrite(BUILTIN_LED, HIGH);
  debugln("GRN_Btn_Send_EXIT");
}

void BLU_Btn_Send()
{
  if (IN_ISR)
    return;
  IN_ISR = true;
  debugln("BLU_Btn_Send");
  digitalWrite(BUILTIN_LED, LOW);
  actSend = (uint8_t *)B__;
  digitalWrite(BUILTIN_LED, HIGH);
  debugln("BLU_Btn_Send_EXIT");
}

void dispBattery()
{
  debugln("Powering Battery LEDs");
  prevMilBat = millis();
  if (getBattery)
    digitalWrite(REMOTE_BAT_LED, HIGH);
  if (getRemBattery)
    digitalWrite(VEHICLE_BAT_LED, HIGH);
}

bool getBattery()
{
  float measuredvbat = analogRead(VBATPIN);
  /*measuredvbat *= 2;    // we divided by 2, so multiply back
    measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage
    debug("VBat: " ); debugln(measuredvbat);
  */
  measuredvbat = map(measuredvbat, 465, 650, 0, 100);
  debug("VBat A: " ); debugln(measuredvbat);
  debug("VBat %: " ); debugln(measuredvbat);
  return measuredvbat < 20 ? true : false;
}
