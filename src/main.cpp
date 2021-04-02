#include <Arduino.h>
#include <control_connection.h>
#include <ftp_server.h>

const char* ssid = "<your internal network ssid>";
const char* password = "<your internal network password>"

const char* server_password = "<set password for server>";
const int server_delay_time = 50;

ftp_server server;

void setup(){
    Serial.begin(9600);
    ftp_init(&server, ssid, password, server_password, server_delay_time);
    Serial.println(WiFi.localIP());
}

void loop(){
    ftp_loop(&server);
}