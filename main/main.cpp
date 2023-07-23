#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <esp_log.h>
#include "InputsOutputs.hpp"
#include "TcpService.hpp"

using namespace std;

/*Memory size*/
#define KB 1024
#define MB (KB)(KB)

static const char *MAIN_TAG = "Main";

InputsOutputs *Gpio;
TcpService *Tcp;

void logString(const char *TAG, const char *message)
{
    ESP_LOGI(TAG, "%s", (unsigned char *)message);
}

void logDword(const char *TAG, uint32_t logNumber)
{
    char charNumber[KB];
    sprintf(charNumber, "%d", logNumber);
    logString(TAG, charNumber);
}

void logFloat(const char *TAG, double logFloating)
{
    char charFloating[KB];
    sprintf(charFloating, "%lf", logFloating);
    logString(TAG, (char *)charFloating);
}

void initObjects()
{
    Gpio = new InputsOutputs();
    Gpio->logString = logString;
    Gpio->logDword = logDword;
    Gpio->logFloat = logFloat;
    Gpio->InitBlink();

    Tcp = new TcpService();
    Tcp->InitTcpService(WiFiMode::Ap);
    Tcp->logString = logString;
    Tcp->logDword = logDword;
    Tcp->logFloat = logFloat;
}

extern "C" int app_main(void)
{
    logString(MAIN_TAG, "Go Proyect!");
    initObjects();
    return 0;
}
