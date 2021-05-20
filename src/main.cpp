#include <Arduino.h>
#include "tamper.h"
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
const char* SD_FAILURE_MSG = "Failed to mount SD card. FTP service unavailable.";

const int TAMPER_PIN = 34; /* Any pin that can be input will work */
const char* SENSITIVE_FILE_PATH = "<path to file containing sensitive data>";

tamper_task_data task_data;
FtpServer server(serverPassword, serverDelayTime, rootPath);

void setup(){
    Serial.begin(9600);
    if(!SD.begin(SD_CS_PIN) || SD.cardType() == CARD_NONE){
        Serial.println(SD_FAILURE_MSG);
        return;
    }
    server.init(ssid, password);
    setup_tamper_isr(TAMPER_PIN, &task_data, SENSITIVE_FILE_PATH);
    Serial.println(WiFi.localIP());
}

void loop(){
    server.loop();
}
