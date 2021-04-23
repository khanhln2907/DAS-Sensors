/*
 Name:		TestGPSBlox7.ino
 Created:	4/23/2021 3:02:36 PM
 Author:	Khanh Le
*/

#include <TinyGPS.h>
#include <CircularBuffer.h>

#define GPS_PORT Serial2
TinyGPS myGPSHandler;
CircularBuffer<uint8_t, 100> gpsBuffer;

void setup() {
    Serial.begin(115200);
    //pinMode(16, INPUT_PULLUP);
    GPS_PORT.begin(9600);
}

void loop() {
    float flat, flon;
    unsigned long age, date, time, chars = 0;
    unsigned short sentences = 0, failed = 0;
    static unsigned long lastTime = millis() ;

    auto nRx = GPS_PORT.available();
    if(nRx)
        Serial.printf("%d Bytes Receveid \n", nRx);
    while (nRx > 0) {
        uint8_t rxByte = GPS_PORT.read();
        myGPSHandler.encode(rxByte);
        Serial.print(rxByte);
        Serial.print(" ");
        nRx--;
    }

    if (millis() - lastTime > 1000) {
        myGPSHandler.f_get_position(&flat, &flon, &age);
        myGPSHandler.stats(&chars, &sentences, &failed);
        Serial.println();
        Serial.printf("Lat: %.3f \t Lon: %.3f \t Age: %d \n", flat, flon, age);
        Serial.printf("Char: %d \t Sen: %d \t Failed: %d \n \n", chars, sentences, failed);
        lastTime = millis();
    }
}