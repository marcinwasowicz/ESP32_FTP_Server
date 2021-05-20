#include "Security.h"

Security::Security(String sensitiveFilePath)
    : triggered(false), sensitiveFilePath(std::move(sensitiveFilePath))
{
    this->semaphoreHandle = xSemaphoreCreateBinary();
}

Security::~Security()
{
    vSemaphoreDelete(this->semaphoreHandle);
}

bool Security::getTriggered() const
{
    return this->triggered;
}

void Security::setTriggered(bool trigger)
{
    this->triggered = trigger;
}

const String& Security::getSensitiveFilePath() const
{
    return this->sensitiveFilePath;
}

void Security::upSemaphore()
{
    xSemaphoreGiveFromISR(this->semaphoreHandle, NULL);
}

void Security::downSemaphore()
{
    xSemaphoreTake(this->semaphoreHandle, portMAX_DELAY);
}

[[noreturn]] void Security::securityTask(Security &security)
{
    for(;;)
    {
        security.downSemaphore();
        security.setTriggered(true);
        if(security.getSensitiveFilePath().length() && SD.exists(security.getSensitiveFilePath()))
        {
            SD.remove(security.getSensitiveFilePath());
        }
        else
        {
            vTaskDelete(nullptr);
        }
    }
}

void Security::securityISR(Security &security)
{
    if(!security.getTriggered())
    {
        security.upSemaphore();
    }
}

void Security::setupSecurityISR(int securityPin, Security &security)
{
    pinMode(securityPin, INPUT);
    xTaskCreate((TaskFunction_t) Security::securityTask, "securityTask", 10000, &security, 1000, nullptr);
    attachInterruptArg(securityPin, (void (*)(void *)) Security::securityISR, &security, CHANGE);
}




