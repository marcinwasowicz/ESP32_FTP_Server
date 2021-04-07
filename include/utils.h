#ifndef ESP32_FTP_SERVER_UTILS_H
#define ESP32_FTP_SERVER_UTILS_H

#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <control_connection.h>

int get_socket_from_command(String& command, client_struct* client);

bool check_file_status(const char* path);

bool make_directory(const char* path);

void get_dir_listing(File dir, String& listing_buffer);

#endif
