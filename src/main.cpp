#include <Arduino.h>
#include <ftp_server.h>
#include <utils.h>
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

const char* server_password = "<set password for server>";
const char* root_path = "/";
const int server_delay_time = 50;

const int SD_CS_PIN = 5;
const char* SD_FAILURE_MSG = "Failed to mount SD card. FTP service unavailable.";

ftp_server server;

void setup(){
    Serial.begin(9600);
    if(!SD.begin(SD_CS_PIN) || SD.cardType() == CARD_NONE){
        Serial.println(SD_FAILURE_MSG);
        return;
    }
    ftp_init(&server, ssid, password, server_password, server_delay_time, root_path);
    Serial.println(WiFi.localIP());
}

void loop(){
    ftp_loop(&server);
}