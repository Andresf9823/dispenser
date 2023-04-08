#ifndef _TCP_SERVICE_HPP
#define _TCP_SERVICE_HPP

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/freertos.h>
#include "freertos/event_groups.h"
#include <freertos/task.h>
#include <freertos/timers.h>
#include <esp_log.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_wifi.h>

class TcpService
{
private:
    const char *tag = "Tcp";
    uint16_t maximumSizeOfScanList;

public:
    TcpService();
    void InitTcpService();
    void ScanWifiNetworks();
    void (*logString)(const char *TAG, const char *message);
    void (*logDword)(const char *TAG, uint32_t logNumber);
    void (*logFloat)(const char *TAG, float logFloating);
    ~TcpService();
};
#endif