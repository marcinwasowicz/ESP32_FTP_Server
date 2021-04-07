#ifndef ESP32_FTP_SERVER_CONTROL_CONNECTION_H
#define ESP32_FTP_SERVER_CONTROL_CONNECTION_H

#include <WiFi.h>
#include <ftp_server.h>

void send_reply_code(client_struct* client, const char* code);

void greet_client(client_struct* client);

void handle_user(client_struct* client, String& command);

void handle_password(client_struct* client, const char* server_password, String& command);

void handle_quit(client_struct* client);

void handle_set_data_socket(client_struct* client, String& command);

void handle_dir_listing(client_struct* client, String& command, ftp_server* server);

void handle_print_working_directory(client_struct* client, ftp_server* server);

void handle_type(client_struct* client);

void handle_retrieve_file(client_struct* client, ftp_server* server, String& command);

void listen_command(client_struct* client, ftp_server* server);

void shut_down_ctrl_connection(client_struct* client);

#endif
