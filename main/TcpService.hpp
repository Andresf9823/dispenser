#ifndef _TCP_SERVICE_HPP
#define _TCP_SERVICE_HPP

#include <GlobalDefines.hpp>
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

typedef enum protocolCommand
{
    sendDeviceInfo = 0x0B,
    sendWifiApRecords = 0x1B,
} ProtocolCommand;

static uint8_t *tcpBuffer[RX_BUFFER_SIZE];

class TcpService
{
private:
    const char *tag = "TCP";
    static void serverTask(void *pvParameters);
    static void do_retransmit(const int sock);
    static bool isValidFrame(uint8_t *frame);

public:
    TcpService(/* args */);
    void CreateSocket(uint16_t port);
    static void (*SendWifiApRecordsScanned)(void);
    void (*logString)(const char *TAG, const char *message);
    void (*logDword)(const char *TAG, uint32_t logNumber);
    void (*logFloat)(const char *TAG, double logFloating);

    static void TcpAppStack(uint8_t *buffer);

    ~TcpService();
};

#endif