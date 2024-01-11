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

#define CommandSendWifiApRecords 0x1B

static char tcpBuffer[RX_BUFFER_SIZE];

class TcpService
{
private:
    static constexpr char *tag = "TCP";
    static void serverTask(void *pvParameters);
    static void do_retransmit(const int sock);
    static bool isValidFrame(uint8_t *frame, uint len);

public:
    TcpService(/* args */);
    void CreateSocket(uint16_t port);
    void TcpAppStack();
    void SendTcpMessage(char *message);
    void (*SendWifiApRecordsScanned)(void);

    void (*logString)(const char *TAG, const char *message);
    void (*logDword)(const char *TAG, uint32_t logNumber);
    void (*logFloat)(const char *TAG, double logFloating);

    ~TcpService();
};

#endif