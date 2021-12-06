

#include "LoRa.h"
#include "debug.h"

// Singleton instance of the radio driver
RH_RF95 driver(8, 3); // Adafruit Feather M0 with RFM95

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, VEHICLE_ADDRESS);

// Dont put this on the stack:
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];

uint8_t ack[] = "ACK";

void LoRa_Setup()
{
  if (!manager.init())
    debugln("init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // you can set transmitter powers from 2 to 20 dBm:
  //  driver.setTxPower(20, false);

}

uint8_t* LoRa_Read()
{
  if (manager.available())
  {
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAck(buf, &len, &from))
    {
      debug("Vehicle: got request from : 0x");
      debug(from, 0); //, HEX);
      debug(": ");
      debug((char*)buf);
      debug(" : ");
      debugln(len, 0);

      // Send a reply back to the originator client
      if (!manager.sendtoWait(ack, sizeof(ack) * 2, from))
        debugln("sendtoWait failed");


      return buf;
    }
  }
  return (uint8_t*)"";
}
