#ifndef ESP32_FTP_SERVER_DATA_CONNECTION_H
#define ESP32_FTP_SERVER_DATA_CONNECTION_H


#include <ftp_server.h>
#include <utils.h>

const int MAX_BYTE_RETRIES = 2;

bool open_data_connection(ftp_server* server, client_struct* client);

void send_dir_listing(ftp_server* server, const char* path);

bool send_file(ftp_server* server, const char* path);

bool store_file(ftp_server* server, String& path);

void close_data_connection(ftp_server* server);

#endif
