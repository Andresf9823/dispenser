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

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

class TcpService
{
private:
    const char *tag = "TCP";
    static void serverTask(void *pvParameters);
    static void do_retransmit(const int sock, bool connectionState);

public:
    TcpService(/* args */);
    void CreateSocket(uint16_t port);

    void (*logString)(const char *TAG, const char *message);
    void (*logDword)(const char *TAG, uint32_t logNumber);
    void (*logFloat)(const char *TAG, double logFloating);
    ~TcpService();
};

#endif