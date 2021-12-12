#include "LoRa.h"
#include "debug.h"

#define DEBUG_ENABLE

byte BUILTIN_LED = 13;

byte OPEN_ANT_LED = 14;
byte CLOSE_ANT_LED = 15;

byte OPEN_ANT_BTN = 6;  //check available pin
byte CLOSE_ANT_BTN = 5;

byte RED_BTN = 10;
byte GRN_BTN = 11;
byte BLU_BTN = 12;

bool IN_ISR = false;

uint8_t * actSend, * actRecv;
char OPEN_ACT[] = "ANT_OPEN";
char CLOSE_ACT[] = "ANT_CLOS";
uint8_t R__[] = "R++";
uint8_t G__[] = "G++";
uint8_t B__[] = "B++";

unsigned long prevMillis;

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
    IN_ISR = false;
  }

  Check_Action();

  if (prevMillis != 0)
    if (millis() - prevMillis >= 10000)
    {
      debugln("powering off LEDs");
      prevMillis = 0;
      digitalWrite(OPEN_ANT_LED, LOW);
      digitalWrite(CLOSE_ANT_LED, LOW);
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
  else if (strcmp((char*)actRecv , "OPEN_ACK") == 0)
  {
    debugln("LoRa RECV: OPEN_ACK");
    Set_Open_LED();
  }
  else if (strcmp((char*)actRecv , "CLOS_ACK") == 0)
  {
    debugln("LoRa RECV: CLOS_ACK");
    Set_Close_LED();
  }
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
