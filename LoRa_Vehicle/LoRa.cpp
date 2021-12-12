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

int LoRa_Send(uint8_t * data)
{
  // Send a message to manager_server
  if (manager.sendtoWait(data, sizeof(data) * 2, REMOTE_ADDRESS))
  {
    
    debug("Sending: ");
    debug((char*)data);
    debug(" : to: 0x");
    debug(REMOTE_ADDRESS,0);
    debug(" : ");
    debugln(sizeof(data) * 2);

    // Now wait for a reply from the server
    uint8_t len = sizeof(buf) * 2;
    uint8_t from;
    if (manager.recvfromAckTimeout(buf, &len, 2000, &from))
    {
      debug("Remote: got reply from : 0x");
      debug(from, 0); // HEX);
      debug(": ");
      debugln((char*)buf);
      if ((char*)buf == "ACK")
        return 0;
      else
        return 1;
    }
    else
    {
      debugln("No reply, is rf95_reliable_datagram_server running?");
      return 2;
    }
  }
  else
  {
    debugln("sendtoWait failed");
    return -1;
  }
}
