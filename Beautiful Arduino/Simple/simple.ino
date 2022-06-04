#include "teseo_liv3f_class.h"
#include "LoRaWAN.h"
#include "TimerMillis.h"
#include <CayenneLPP.h>

const char *devEui = "6081F9D499665A68";
const char *appEui = "6081F99CD641F555";
const char *appKey = "78E6CA47EF943377E4D6FF293DD149F7";

uint32_t  GLOBAL_COUNTER;
uint32_t  ACK_COUNTER;
uint32_t  NACK_COUNTER;

CayenneLPP lpp(51);

void setup( void )
{

    Serial.begin(115200);
    Serial.println("Setup begin");
    GLOBAL_COUNTER = 0;
    
    ACK_COUNTER = 0;
    NACK_COUNTER = 0;
    LoRaWAN.begin(US915);
    LoRaWAN.joinOTAA(appEui, appKey, devEui);

    Serial.println("Setup finish");
}

void loop( void )
{
    if (LoRaWAN.joined() && !LoRaWAN.busy())
    {
            Serial.print("TRANSMIT( ");
            Serial.print("UpLinkCounter: [");
            Serial.print(LoRaWAN.getUpLinkCounter());
            Serial.print("], DownLinkCounter: [");
            Serial.print(LoRaWAN.getDownLinkCounter());
            Serial.println("])");
            
            GLOBAL_COUNTER += 1;
            Serial.print("GLOBAL_COUNTER: [");
            Serial.print(GLOBAL_COUNTER);
            Serial.println("]");

            lpp.reset();
            lpp.addPresence(1, GLOBAL_COUNTER);

            LoRaWAN.sendPacket(1, lpp.getBuffer(), lpp.getSize(), false);
            
            //Reproduce Table
           
            delay(5000);  
    } 
}