#ifndef ESP32_FTP_SERVER_TAMPER_H
#define ESP32_FTP_SERVER_TAMPER_H

#include <FS.h>
#include <SD.h>
#include <SPI.h>

struct tamper_task_data {
    volatile bool triggered;
    String sensitive_file_path;
};

void setup_tamper_isr(int tamper_pin, tamper_task_data* data, const char* sensitive_file_path);

void IRAM_ATTR tamper_isr(tamper_task_data* data);

[[noreturn]] void tamper_task(tamper_task_data* data);

#endif
