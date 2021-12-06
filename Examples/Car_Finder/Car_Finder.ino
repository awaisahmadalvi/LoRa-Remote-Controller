#include <NewPing.h>
//#include <SPI.h>
#include <RH_RF95.h> //Radio head library

//Ultrasonic sensor
#define TRIG_PIN 5
#define ECHO_PIN 6

// LoRa module RFM95
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
#define RF95_FREQ 915.0

int LS1_BtnPin = A1; //limit switch 1
int LS1_BtnState=0;
int LS2_BtnPin =A2;  //limit switch 2
int LS2_BtnState=0;

int Mtr_In1 = 9; //Motor pins
int Mtr_In2 = 10;
int Mtr_In3 = 12;
int Mtr_In4 = 13;

#define MAX_DISTANCE 500
String msg;             // variable to receive data from transmitter
int distance = 0;
int tag=0;
int val = 0;
int ANT_State = 0;

unsigned long  TIME_OUT=3000;
unsigned long  Current_millis =0;
unsigned long  Previous_millis =0;

NewPing sonar = NewPing(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

RH_RF95 rf95(RFM95_CS, RFM95_INT); // radio driver

void setup() 
{
	pinMode(RFM95_RST, OUTPUT);
	digitalWrite(RFM95_RST, HIGH);
	pinMode(LS1_BtnPin, INPUT); 
	pinMode(LS2_BtnPin, INPUT); 

	pinMode(Mtr_In1, OUTPUT);
	pinMode(Mtr_In2, OUTPUT);
	pinMode(Mtr_In3, OUTPUT);
	pinMode(Mtr_In4, OUTPUT);

	pinMode(Mtr_In1, LOW);
	pinMode(Mtr_In2, LOW);
	pinMode(Mtr_In3, LOW);
	pinMode(Mtr_In4, LOW);

	Serial.begin(115200);
	while (!Serial) 
	{
		delay(1);
	}
	delay(1);

	// manual reset
	digitalWrite(RFM95_RST, LOW);
	delay(1);
	digitalWrite(RFM95_RST, HIGH);
	delay(1);

	while (!rf95.init())
	{
		while (1);
	}
	if (!rf95.setFrequency(RF95_FREQ)) 
	{
		while (1);
	}
	// you can set transmitter powers from 5 to 23 dBm:
	rf95.setTxPower(23, false);
}

void loop() 
{   

	uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
	uint8_t len = sizeof(buf); 

	LS1_BtnState = digitalRead(LS1_BtnPin);  //read limit switch 1
	LS2_BtnState = digitalRead(LS2_BtnPin);  //read limit switch 2

	if (rf95.available()&& rf95.recv(buf, &len) ) 
	{
		msg = ((char*)buf);  // received msg from transmitter
	}

	if (msg == "1234567891234567")  //for upward direction
	{
		//Serial.println("Upward direction");
		Current_millis= millis();
		distance = sonar.ping_cm();
		if (distance <=21)
		{
			distance=21; 
		} 
		if (distance >21 && distance <=134)
		{ 
			Serial.println(" Motor Is OFF");
		}   
		else if (distance > 134 )     
		{

			if (LS1_BtnState == HIGH && val==0)  //limit switch 1 not press
			{
				Serial.println(" Motor Is ON");
				digitalWrite(Mtr_In1, HIGH);
				digitalWrite(Mtr_In2, LOW);
				digitalWrite(Mtr_In3, LOW);
				digitalWrite(Mtr_In4, LOW);
				delay(100);
				val=1;
			}
			else if(LS1_BtnState == LOW && val==1)      //limit switch 1 pressed
			{
				val=0;
				Serial.println(" Motor Is OFF");
				digitalWrite(Mtr_In1, LOW);
				digitalWrite(Mtr_In2, LOW);
				Previous_millis = Current_millis;
				Serial.println("antenna is moving up");
				digitalWrite(Mtr_In3, HIGH);
				digitalWrite(Mtr_In4, LOW);
				delay(100);
				ANT_State=1;
			} 
		} 
	}   
	 if (msg == "987654321" )  //for downward direction
	{
	Current_millis = millis();
	Serial.println("antenna is moving Down"); //Problem is here motor not run in reverse directio
 Serial.println(Current_millis);
  Previous_millis = Current_millis;
	ANT_State=2;
	}
	
	switch(ANT_State)
	{
	case 1:
	{
  	if((Current_millis-Previous_millis) > TIME_OUT) //timer stopped here for antenna motor
  	{
    	Serial.println("antenna is moving up Stopped");
    	digitalWrite(Mtr_In3, LOW);
    	digitalWrite(Mtr_In4, LOW);
    	digitalWrite(Mtr_In1, LOW);
    	digitalWrite(Mtr_In2, LOW);
    	Previous_millis = Current_millis;
    	ANT_State=0;
  	}   
	break;
	}
	case 2:
	{    
	if((Current_millis-Previous_millis) <= TIME_OUT)
	{
  	Serial.println("antenna is moving Down stopped");
  	digitalWrite(Mtr_In3, LOW);
  	digitalWrite(Mtr_In4, HIGH);
  	digitalWrite(Mtr_In1, LOW);
  	digitalWrite(Mtr_In2, LOW);
  	Previous_millis = Current_millis;
   ANT_State=3;
	}
	break;    
	}
	default:
	break;
	}

}
