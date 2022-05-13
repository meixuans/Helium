#include "LoRaWAN.h"

const char *devEui = "6081F9D499665A68";
const char *appEui = "6081F99CD641F555";
const char *appKey = "78E6CA47EF943377E4D6FF293DD149F7";

// Max Payload 53 Bytes for DR 1
const uint8_t payload[] = "Hello, World! Hello, World!";
uint8_t incomingPacket[255]; //to read

void setup( void )
{
    Serial.begin(9600);
    
    while (!Serial) { }

    // US Region
    LoRaWAN.begin(US915);
    // Helium SubBand
    LoRaWAN.setSubBand(2);
    // Disable Adaptive Data Rate
    LoRaWAN.setADR(false);
    // Set Data Rate 1 - Max Payload 53 Bytesc
    LoRaWAN.setDataRate(1);

    //int temp111 = LoRaWAN.setDownLinkDwellTime(true);
//    int temp111 = LoRaWAN.setReceiveDelay(100);
//    Serial.println(temp111);
//    Serial.println("noooo\n");
    
    // Device IDs and Key
    LoRaWAN.joinOTAA(appEui, appKey, devEui);

    Serial.println("JOIN( )");
    LoRaWAN.onJoin(joinCallback);
    //LoRaWAN.onTransmit(call_back_receive);
    //LoRaWAN.onLinkCheck(call_back_receive);
    
}

void loop( void )
{
    Serial.print("hi\n");
    if (LoRaWAN.joined() && !LoRaWAN.busy())
    {
        LoRaWAN.onJoin(temp2);
        Serial.print("TRANSMIT( ");
        Serial.print("TimeOnAir: ");
        Serial.print(LoRaWAN.getTimeOnAir());
        Serial.print(", NextTxTime: ");
        Serial.print(LoRaWAN.getNextTxTime());
        Serial.print(", MaxPayloadSize: ");
        Serial.print(LoRaWAN.getMaxPayloadSize());
        Serial.print(", DR: ");
        Serial.print(LoRaWAN.getDataRate());
        Serial.print(", TxPower: ");
        Serial.print(LoRaWAN.getTxPower(), 1);
        Serial.print("dbm, UpLinkCounter: ");
        Serial.print(LoRaWAN.getUpLinkCounter());
        Serial.print(", DownLinkCounter: ");
        Serial.print(LoRaWAN.getDownLinkCounter());
        Serial.println(" )");

        //to test:
        //getMaxPacketLength()
        //maybe set datarate to 2 and see what happens
        //datarate unchanged: add data length
        //data length unchanged: add data rate

        //todo:
        //write geolocation application
        //

        // Send Packet
        LoRaWAN.sendPacket(1, payload, sizeof(payload), true); 
        LoRaWAN.onReceive(call_back_receive); //maybe check the sequence number first?
    }

    delay(2000); //2 Seconds
}


void temp1 (void) {
  Serial.print("JOINnnnnnnnn1\n");
}
void temp2 (void) {
  Serial.print("JOINnnnnnnnn2\n");
}

void joinCallback(void)
{
    int temp111 = LoRaWAN.setReceiveDelay(200);
    Serial.println(temp111);
}

void call_back_receive(void) {
    int packetSize = LoRaWAN.parsePacket();
    Serial.print("Received packet: \n");
    //read packet
    for (int i = 0; i < packetSize; i++) {
      Serial.print("SUCCESS\n");
      int temp = LoRaWAN.read(incomingPacket, 255);
      Serial.print("IncomingPacket (before):\n");


      //to read
      //Serial.printf("%" PRIu8"\n", *incomingPacket);
      for (int j = 0; j < temp; j++) {
        Serial.print(incomingPacket[j]);
      }
      //Serial.print(incomingPacket);
      if (temp > 0){ 
         incomingPacket[temp] = 0;//clear buffer
      }
      //Serial.print("IncomingPacket (after clearing buffer): %s\n", incomingPacket);
    }
}
