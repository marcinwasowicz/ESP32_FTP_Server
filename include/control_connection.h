#ifndef ESP32_FTP_SERVER_CONTROL_CONNECTION_H
#define ESP32_FTP_SERVER_CONTROL_CONNECTION_H

#include <WiFi.h>

struct client_struct{
    WiFiClient connection;
    String name;
    bool logged_in;
    int data_port;
};

void send_reply_code(client_struct* client, const char* code);

void greet_client(client_struct* client);

void handle_user(client_struct* client, String& command);

void handle_password(client_struct* client, const char* server_password, String& command);

void handle_quit(client_struct* client);

void handle_set_data_port(client_struct* client, String& command);

void listen_command(client_struct* client, const char* server_password);

void shut_down_ctrl_connection(client_struct* client);

#endif
