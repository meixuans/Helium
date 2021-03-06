#include "teseo_liv3f_class.h"
#include "LoRaWAN.h"
#include "TimerMillis.h"
#include <CayenneLPP.h>

const char *devEui = "6081F9D499665A68";
const char *appEui = "6081F99CD641F555";
const char *appKey = "78E6CA47EF943377E4D6FF293DD149F7";

uint32_t GLOBAL_COUNTER;
int SEND_PACKET_INTERVAL;
TimerMillis transmitTimer;
TeseoLIV3F* gps;
GNSSParser_Data_t data;
CayenneLPP lpp(51);

#ifdef ARDUINO_ARCH_STM32
HardwareSerial Serial1(PA10, PA9);
#endif

#if defined(ARDUINO_SAM_DUE)
#include <avr/dtostrf.h>
#elif defined(ARDUINO_ARCH_STM32L0)
#include <avr/dtostrf.h>
#endif

void call_back_join(void) {
    if (LoRaWAN.joined()) {
        Serial.println("Joined");
        transmitTimer.start(call_back_transmit, 0, SEND_PACKET_INTERVAL);
    } else {
        Serial.println("rejoin begin - call back join");
        LoRaWAN.rejoinOTAA();
    }
}

void call_back_receive(void) {
    //nothing to receive
}

void call_back_transmit(void) {
    if (!LoRaWAN.busy()) {
        if (!LoRaWAN.linkGateways()) {
            transmitTimer.stop();
            Serial.println("rejoin begin - call back transmit");
            LoRaWAN.rejoinOTAA();
        }
        
        if (LoRaWAN.joined()) {
            GLOBAL_COUNTER++;
            /*
            Serial.print("TRANSMIT( ");
            Serial.print("TimeOnAir: ");
            Serial.print(LoRaWAN.getTimeOnAir());
            Serial.print("dbm, UpLinkCounter: ");
            Serial.print(LoRaWAN.getUpLinkCounter());
            Serial.print(", DownLinkCounter: ");
            Serial.print(LoRaWAN.getDownLinkCounter());
            Serial.println(" )");
            */
            data = gps->getData();

            if (data.gpgga_data.valid == 1) {
                int lat_d = (int) (data.gpgga_data.xyz.lat/100.0);
                int lat_m = (int) (data.gpgga_data.xyz.lat) - (lat_d * 100);
                int lon_d= (int) (data.gpgga_data.xyz.lon/100.0);
                int lon_m = (int) (data.gpgga_data.xyz.lon) - (lon_d * 100);
                
                int lat_int = (int) (data.gpgga_data.xyz.lat);
                int lon_int = (int) (data.gpgga_data.xyz.lon);
                float lat_s = (data.gpgga_data.xyz.lat - lat_int);
                float lon_s = (data.gpgga_data.xyz.lon - lon_int);

                float lat_f = 1.0*lat_d + lat_m/60.0 + lat_s/60.0;
                float lon_f = 1.0*lon_d + lon_m/60.0 + lon_s/60.0;
                
                if (data.gpgga_data.xyz.ns > 80) {
                    lat_f = lat_f*(-1);
                }

                if (data.gpgga_data.xyz.ew > 80) {
                    lon_f = lon_f*(-1);
                }

                lpp.reset();
                lpp.addGPS(1, lat_f, lon_f, 2);
                lpp.addPresence(2, GLOBAL_COUNTER);
                
                Serial.print("GLOBAL_COUNTER: ");
                Serial.println(GLOBAL_COUNTER);
                Serial.print("Current location, LAT: ");
                Serial.print(lat_f);
                Serial.print("LON: ");
                Serial.println(lon_f);
                
                //Reproduce Table
                LoRaWAN.sendPacket(1, lpp.getBuffer(), lpp.getSize(), true);
                while (LoRaWAN.busy()) {
                }
                
                if (LoRaWAN.confirmed()) {
                    Serial.println("ACK received");
                } else {
                    Serial.println("NACK");
                }
                
            } else {
                Serial.println("GPS location not valid");
            }
        }
    }
}

void setup(void) {
    //Serial - User
    //Serial - device
    Serial.begin(115200);
    Serial1.begin(9600);
    Serial.println("Setup begin");
    
    gps = new TeseoLIV3F(&Serial1, 7, 13);
    gps->init();

    LoRaWAN.begin(US915);

    GLOBAL_COUNTER = 0;
    SEND_PACKET_INTERVAL = 2000;

    LoRaWAN.onJoin(call_back_join);
    LoRaWAN.onTransmit(call_back_receive);
    LoRaWAN.onReceive(call_back_transmit);
    LoRaWAN.joinOTAA(appEui, appKey, devEui);

    Serial.println("Setup finish");
}

void loop(void) {
    //set up a timer to call TRANMIT function
    //loop will handle GPS update, at lease 20 times per second
    gps->update();
    //delay(100);
}
