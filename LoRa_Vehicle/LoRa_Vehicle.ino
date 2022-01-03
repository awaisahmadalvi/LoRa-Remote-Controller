#include "LoRa.h"
#include "debug.h"
#include "Ultrasonic.h"
#include "neo.h"

#define DEBUG_ENABLE

#define VBATPIN A7    // 9

//Ultrasonic sensor
#define ANT_OPEN_TIME 7500
#define ANT_HEIGHT 140
int distance = 0;

//const int TRIG_PIN = 10; // Trigger Pin of Ultrasonic Sensor
//const int ECHO_PIN = 11; // Echo Pin of Ultrasonic Sensor

#define ANT_MTR_1       14
#define ANT_MTR_2       15

#define FOLD_MTR_1      16
#define FOLD_MTR_2      17

#define UNFOLD_LMT_SW   18
#define FOLD_LMT_SW     19

byte RED_PWM = 255;
byte GRN_PWM = 255;
byte BLU_PWM = 255;

uint8_t *actRecv, *actSend;

void setup()
{
#ifdef DEBUG_ENABLE
  Serial.begin(115200);
  //while (!Serial) ; // Wait for serial port to be available
#endif

  LoRa_Setup();
  US_setup();

  pinMode(UNFOLD_LMT_SW, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(UNFOLD_LMT_SW), Stop_Folding, FALLING );

  pinMode(FOLD_LMT_SW, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(FOLD_LMT_SW), Stop_Folding, FALLING );

  pinMode(FOLD_MTR_1, OUTPUT);
  pinMode(FOLD_MTR_2, OUTPUT);

  pinMode(ANT_MTR_1, OUTPUT);
  pinMode(ANT_MTR_2, OUTPUT);
}

void loop()
{
  Check_Action();
  delay(20);
}

void Check_Action()
{
  //action = 0;
  actRecv = LoRa_Read();
  //debug("return: ");
  //debugln((char*)actRecv);

  if (strcmp((char*)actRecv, "") == 0)
    return;
  else if (strcmp((char*)actRecv, "ANT_OPEN") == 0)
  {
    debugln("LoRa RECV: ANT_OPEN");
    Open_Antenna();
  }
  else if (strcmp((char*)actRecv, "ANT_CLOS") == 0)
  {
    debugln("LoRa RECV: ANT_CLOS");
    Close_Antenna();
  }
  else if (strcmp((char*)actRecv, "R++") == 0)
  {
    debugln("LoRa RECV: Red PWM Increment");
    // Increment Red by 64
    RED_PWM = (RED_PWM + 64) % 256;
    RGB_ON();
  }
  else if (strcmp((char*)actRecv, "G++") == 0)
  {
    debugln("LoRa RECV: Green PWM Increment");
    // Increment Green by 64
    GRN_PWM = (GRN_PWM + 64) % 256;
    RGB_ON();
  }
  else if (strcmp((char*)actRecv, "B++") == 0)
  {
    debugln("LoRa RECV: Blue PWM Increment");
    // Increment Blue by 64
    BLU_PWM = (BLU_PWM + 64) % 256;
    RGB_ON();
  }
}

void Open_Antenna()
{
  //byte close = digitalRead(FOLD_LMT_SW);
  //byte open = digitalRead(UNFOLD_LMT_SW);
  debug("FOLD_LMT_SW: ");
  debug(digitalRead(FOLD_LMT_SW), 0);
  debug(", UNFOLD_LMT_SW: ");
  debugln(digitalRead(UNFOLD_LMT_SW), 0);
  if (digitalRead(FOLD_LMT_SW) == LOW) //Opening Antenna Asembly
  {
    /* Check Antenna Clearance */
    if (Check_Clearance()) // If clear then open antenna
    {
      Open_LoRa_Send();
      Unfold_Ant();
      RGB_ON();
      Uncoil_Ant();
    }
  }
  else if (digitalRead(UNFOLD_LMT_SW) == LOW) //Opening Antenna Asembly
  {
    Open_LoRa_Send();
  }

}

void Close_Antenna()
{
  //byte close = digitalRead(FOLD_LMT_SW);
  //byte open = digitalRead(UNFOLD_LMT_SW);
  debug("FOLD_LMT_SW: ");
  debug(digitalRead(FOLD_LMT_SW), 0);
  debug(", UNFOLD_LMT_SW: ");
  debugln(digitalRead(UNFOLD_LMT_SW), 0);
  if (digitalRead(UNFOLD_LMT_SW) == LOW)   // Clossing antenna assembly
  {
    Coil_Ant();
    RGB_OFF();
    Fold_Ant();
  }
  Close_LoRa_Send();
}

void RGB_ON()
{
  debug("Powering on LED: ");
  debug(RED_PWM, 0);
  debug(", ");
  debug(GRN_PWM, 0);
  debug(", ");
  debugln(BLU_PWM, 0);
  LED_ON(RED_PWM, GRN_PWM, BLU_PWM);
}

void RGB_OFF()
{
  debugln("Powering off LED");
  LED_OFF();
}

void Uncoil_Ant()
{
  debugln("Uncoiling Antenna");
  digitalWrite(ANT_MTR_1, HIGH);
  digitalWrite(ANT_MTR_2, LOW);
  delay(ANT_OPEN_TIME);
  digitalWrite(ANT_MTR_1, LOW);
  digitalWrite(ANT_MTR_2, LOW);
}

void Coil_Ant()
{
  debugln("Coiling Antenna");
  digitalWrite(ANT_MTR_1, LOW);
  digitalWrite(ANT_MTR_2, HIGH);
  delay(ANT_OPEN_TIME);
  digitalWrite(ANT_MTR_1, LOW);
  digitalWrite(ANT_MTR_2, LOW);
}

void Fold_Ant()
{
  debugln("Folding Antenna");
  digitalWrite(FOLD_MTR_1, LOW);
  digitalWrite(FOLD_MTR_2, HIGH);
  while (digitalRead(FOLD_LMT_SW) == HIGH);
  Stop_Folding();
}

void Unfold_Ant()
{
  debugln("Unfolding Antenna");
  digitalWrite(FOLD_MTR_1, HIGH);
  digitalWrite(FOLD_MTR_2, LOW);
  while (digitalRead(UNFOLD_LMT_SW) == HIGH);
  Stop_Folding();
}

void Stop_Folding()
{
  debugln("Stop Folding/unfolding Antenna");
  digitalWrite(FOLD_MTR_1, LOW);
  digitalWrite(FOLD_MTR_2, LOW);
}
/*
  void Stop_Unfolding()
  {
  debugln("stop Unfolding Antenna");
  digitalWrite(FOLD_MTR_1, LOW);
  digitalWrite(FOLD_MTR_2, LOW);
  }
*/
bool Check_Clearance()
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

void Open_LoRa_Send()
{
  debugln("OPEN_ACK_Send");
  if ( getBattery())
    actSend = (uint8_t *) "OPN_ACK1";
  else
    actSend = (uint8_t *) "OPN_ACK0";
  LoRa_Send(actSend);
  debugln("OPEN_ACK_Send_EXIT");
}

void Close_LoRa_Send()
{
  debugln("CLOS_ACK_Send");

  if ( getBattery())
    actSend = (uint8_t *) "CLS_ACK1";
  else
    actSend = (uint8_t *) "CLS_ACK0";

  LoRa_Send(actSend);
  debugln("CLOS_ACK_Send_EXIT");
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
  return measuredvbat < 20 ? true : false;
}
