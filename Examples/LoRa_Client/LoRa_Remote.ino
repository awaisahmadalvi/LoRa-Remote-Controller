#include "LoRa.h"
#include "debug.h"

#define DEBUG_ENABLE

byte BUILTIN_LED = 13;

byte ANT_BTN = 5;//9;

byte RED_BTN = 10;
byte GRN_BTN = 11;
byte BLU_BTN = 12;

void setup()
{

#ifdef DEBUG_ENABLE
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
#endif

  LoRa_Setup();
}

uint8_t data[] = "Hello World!";

void loop()
{
  Serial.println("Sending to Vehicle");
  LoRa_Send(data);
  delay(200);
}
