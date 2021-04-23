#include <tamper.h>

void setup_tamper_isr(const int tamper_pin, ftp_server* server){
    pinMode(tamper_pin, INPUT);
    attachInterruptArg(tamper_pin, (void (*)(void *)) tamper_isr, server, CHANGE);
}

void IRAM_ATTR tamper_isr(ftp_server* server){
    ftp_shutdown(server);
}

