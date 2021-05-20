#ifndef ESP32_FTP_SERVER_SECURITY_H
#define ESP32_FTP_SERVER_SECURITY_H

#include <FS.h>
#include <SD.h>
#include <SPI.h>

class Security {
private:
    bool triggered;
    String sensitiveFilePath;
    SemaphoreHandle_t semaphoreHandle;

    void upSemaphore();
    void downSemaphore();

    [[noreturn]] static void securityTask(Security& security);
    static void IRAM_ATTR securityISR(Security& security);

public:
    explicit Security(String sensitiveFilePath);
    ~Security();
    void setTriggered(bool trigger);
    bool getTriggered() const;

    const String& getSensitiveFilePath() const;
    static void setupSecurityISR(int securityPin, Security& security);
};

#endif
