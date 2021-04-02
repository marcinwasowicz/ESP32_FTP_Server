#ifndef ESP32_FTP_SERVER_FTP_SERVER_H
#define ESP32_FTP_SERVER_FTP_SERVER_H

#include <WiFi.h>
#include <control_connection.h>

const int MAX_CLIENTS_COUNT = 2;
const int FTP_CONTROL_PORT = 21;

struct ftp_server{
    int num_clients;
    int delay_time;
    const char* password;

    WiFiServer connection;
    client_struct clients[MAX_CLIENTS_COUNT];
};

void ftp_init(ftp_server* server, const char* ssid, const char* net_password, const char* srv_password, int delay_time);

void ftp_accept(ftp_server* server);

void ftp_handle_client(ftp_server* server, int client_id);

void ftp_loop(ftp_server* server);

void ftp_shutdown(ftp_server* server);

#endif
