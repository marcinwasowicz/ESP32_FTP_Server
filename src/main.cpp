#include <Arduino.h>
#include "Security.h"
#include "FtpServer.h"

/**
 * cheat sheet for seting up circuit with microSD adapter
 * microSD adapter : | ESP32 board:
 * GND               | GND
 * VCC               | 5V
 * MISO              | 19
 * MOSI              | 23
 * SCK               | 18
 * CS                | 5
**/

const char* ssid = "<your internal network ssid>";
const char* password = "<your internal network password>";

const char* serverPassword = "<set password for server>";
const char* rootPath = "/";
const int serverDelayTime = 50;

const int SD_CS_PIN = 5;
const char* sdFailureMessage = "Failed to mount SD card. FTP service unavailable.";

const int SECURITY_PIN = 34; /* Any pin that can be input will work */
const char* sensitiveFilePath = "<path to file containing sensitive data>";

Security security(sensitiveFilePath);
FtpServer server(serverPassword, serverDelayTime, rootPath);

void setup(){
    Serial.begin(9600);
    if(!SD.begin(SD_CS_PIN) || SD.cardType() == CARD_NONE){
        Serial.println(sdFailureMessage);
        return;
    }
    server.init(ssid, password);
    Security::setupSecurityISR(SECURITY_PIN, security);
    Serial.println(WiFi.localIP());
}

void loop(){
    server.loop();
}