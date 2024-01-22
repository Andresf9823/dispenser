#ifndef _TCP_SERVICE_HPP
#define _TCP_SERVICE_HPP

#include <GlobalDefines.hpp>
#include <freertos/FreeRTOS.h>
#include <freertos/freertos.h>
#include "freertos/event_groups.h"
#include <freertos/task.h>
#include <freertos/timers.h>
#include <esp_wifi.h>

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

using namespace std;

#define TCP_RX_BUFFER_SIZE (KB) / 2
#define TCP_TX_BUFFER_SIZE (KB) * (1.5)
#define TCP_TASK_SIZE (KB) * (4)

#define IP_ADDRESS_0 192
#define IP_ADDRESS_1 168
#define IP_ADDRESS_2 0
#define IP_ADDRESS_3 1

#define NETMASK_0 192
#define NETMASK_1 168
#define NETMASK_2 0
#define NETMASK_3 1

#define GATEWAY_0 192
#define GATEWAY_1 168
#define GATEWAY_2 0
#define GATEWAY_3 1

typedef enum protocolCommand
{
    sendDeviceInfo = 0x0B,
    sendWifiApRecords = 0x1B,
} ProtocolCommand;

typedef struct _NetworkIpAddress
{
    string ssid;
    string password;
    uint8_t auth;
    uint8_t mode;
    uint8_t mac[6];
    uint8_t ip[4];
    uint8_t mask[4];
    uint8_t gateway[4];
    bool dhcpEnlabled;
} NetworkIpAddress;

/*TcpSocker buffer and state flag*/
static char tcpBuffer[TCP_RX_BUFFER_SIZE];
static int socketState;

class TcpService
{
private:
    static constexpr string tag = "TCP";
    static bool isValidFrame(char *frame, uint len);
    static void serverTask(void *pvParameters);
    static void do_retransmit(const int sock);

protected:
    esp_netif_t *esp_netif_ap;
    esp_netif_t *esp_netif_sta;
    void SetApIpAddress();
    void SetStaIpAddress();
    void CreateSocket(uint16_t port);

public:
    TcpService(/* args */);
    void TcpAppStack();
    void SendTcpMessage(string message);

    void (*logString)(string TAG, string message);
    void (*logDword)(string TAG, int32_t logNumber);
    void (*logFloat)(string TAG, double logFloating);
    void (*SendWifiApRecordsScanned)(void);
    void (*SendDeviceInfo)(void);

    ~TcpService();
};

#endif