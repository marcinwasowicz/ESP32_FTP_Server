#ifndef ESP32_FTP_SERVER_DATA_CONNECTION_H
#define ESP32_FTP_SERVER_DATA_CONNECTION_H


#include <ftp_server.h>

bool open_data_connection(ftp_server* server, client_struct* client);

void sent_raw_bytes(ftp_server* server, const char* bytes);

void close_data_connection(ftp_server* server);

#endif
