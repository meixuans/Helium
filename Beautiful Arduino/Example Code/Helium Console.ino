/* Simple OTAA join for a LoRaWAN network
 *  
 *  In setup() below please replace the argument to LoRaWAN.begin()
 *  with your appropriate region specific band:
 *
 *  AS923
 *  AU915
 *  EU868
 *  IN865
 *  KR920
 *  US915
 *
 *  AU915/US915 networks have 64+8 channels. Typical gateways support only
 *  8 (9) channels. Hence it's a good idea to pick the proper channel
 *  subset via select via LoRaWAN.setSubBand(),
 *    
 *  EU868/IN865 have duty cycle restrictions. For debugging it makes sense
 *  to disable those via setDutyCycle(false);
 *    
 *  For an external antenna one should set the proper antenna gain
 *  (default is 2.0) via setAntennaGain().
 *    
 *  Please edit the keys below as they are just debugging samples.
 *    
 *    
 * This example code is in the public domain.
 */

#include "LoRaWAN.h"

const char *devEui = "6081F9D499665A68";
const char *appEui = "6081F99CD641F555";
const char *appKey = "78E6CA47EF943377E4D6FF293DD149F7";

void setup( void )
{
    Serial.begin(9600);
    
    while (!Serial) { }

    LoRaWAN.begin(US915);
    LoRaWAN.joinOTAA(appEui, appKey, devEui);

    Serial.println("JOIN( )");
}

void loop( void )
{
    if (LoRaWAN.joined() && !LoRaWAN.busy())
    {
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

        LoRaWAN.beginPacket();
        LoRaWAN.write(0xef);
        LoRaWAN.write(0xbe);
        LoRaWAN.write(0xad);
        LoRaWAN.write(0xde);
        LoRaWAN.endPacket();
    }

    delay(10000);
}
