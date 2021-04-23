#ifndef ESP32_FTP_SERVER_TAMPER_H
#define ESP32_FTP_SERVER_TAMPER_H

#include <ftp_server.h>

void setup_tamper_isr(const int tamper_pin, ftp_server* server);

void IRAM_ATTR tamper_isr(ftp_server* server);

#endif
