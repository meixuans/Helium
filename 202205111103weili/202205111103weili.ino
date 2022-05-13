/* Send a confirmed packet every 10 seconds and print out the ACK/NACK status.
 *
 *  Per default LoRaWAN uses Unconfirmed packets, which of course are subject
 *  to packet loss. However one can request a confirmation from the gateway.
 *  The downside is that network bandwidth is used up for the confirmation,
 *  as well as additional transmissions in case there was no confirmation
 *  received.
 *  
 */

#include "LoRaWAN.h"

const char *devEui = "6081F9D499665A68";
const char *appEui = "6081F99CD641F555";
const char *appKey = "78E6CA47EF943377E4D6FF293DD149F7";

void call_back_join(void) {
    
}

void setup( void )
{
    Serial.begin(9600);

    while (!Serial) { }

    LoRaWAN.begin(US915);
    // LoRaWAN.setSubBand(2);
    // LoRaWAN.setDutyCycle(false);
    // LoRaWAN.setAntennaGain(2.0);
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
        LoRaWAN.endPacket(true);
        
        while (LoRaWAN.busy())
        {
        }
        
        if (LoRaWAN.confirmed())
        {
            Serial.println("ACK");
        }
        else
        {
            Serial.println("NACK");
        }
    }

    delay(10000);
}
