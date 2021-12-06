#ifndef LORA_H
#define LORA_H

#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>


#define REMOTE_ADDRESS 1
#define VEHICLE_ADDRESS 2


void LoRa_Setup();
int LoRa_Send(uint8_t data[]);


#endif
