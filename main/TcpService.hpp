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

#define MAXIMUM_SIZE_OF_SCAN_LIST 10

typedef enum WiFiMode
{
    ApStation,
    Station,
    Ap
} _WifiMode;

class TcpService
{
private:
    const char *tag = "Tcp";
    static void ScanWifiNetworks();
    static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

public:
    TcpService();
    void InitTcpService(WiFiMode mode);

    void (*logString)(const char *TAG, const char *message);
    void (*logDword)(const char *TAG, uint32_t logNumber);
    void (*logFloat)(const char *TAG, double logFloating);
    ~TcpService();
};
#endif