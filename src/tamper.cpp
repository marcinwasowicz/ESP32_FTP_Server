#include <tamper.h>

void setup_tamper_isr(const int tamper_pin, tamper_task_data* data, const char* sensitive_file_path){
    pinMode(tamper_pin, INPUT);
    data->triggered = false;
    data->sensitive_file_path = String(sensitive_file_path);
    attachInterruptArg(tamper_pin, (void (*)(void *))tamper_isr, data, CHANGE);
}

// TODO : Create a task in main.cpp that waits on mutex and raise this mutex in isr.
void tamper_isr(tamper_task_data* data){
    if(!data->triggered){
        xTaskCreate((TaskFunction_t) (tamper_task), "tamper_task", 10000, data, 1, nullptr);
    }
}

[[noreturn]] void tamper_task(tamper_task_data* data){
    data->triggered = true;
    for(;;){
        if(data->sensitive_file_path.length() && SD.exists(data->sensitive_file_path)){
            SD.remove(data->sensitive_file_path);
        }
        else{
            vTaskDelete(nullptr);
        }
    }
}

