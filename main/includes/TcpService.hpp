#ifndef _TCP_SERVICE_HPP
#define _TCP_SERVICE_HPP

#include <GlobalDefines.hpp>
#include <freertos/FreeRTOS.h>
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

typedef enum protocolCommand
{
    restartSystem = 0x00,
    sendDeviceInfo = 0x0B,
    sendWifiApRecords = 0x1B,
    setDefaultMemoryValues = 0x0C
} ProtocolCommand;

typedef struct _NetworkIpAddress
{
    string ssid;
    string password;
    uint8_t auth;
    uint8_t mode;
    uint8_t mac[6];
    uint8_t apMac[6];
    uint8_t ip[4];
    uint8_t mask[4];
    uint8_t gateway[4];
    bool dhcpEnlabled;
} NetworkIpAddress;

typedef enum _NetworkInterface
{
    Ethernet,
    WifiAp,
    WifiStation,
} NetworkInterface;

/*TcpSocker buffer and state flag*/
static char tcpBuffer[TCP_RX_BUFFER_SIZE];
static int socketState;

class TcpService
{
private:
    static constexpr string tag = "TCP";
    static bool isValidFrame(char *frame, uint len);
    static void serverTask(void *pvParameters);
    static void DoRetransmit(const int sock);

protected:
    esp_netif_t *esp_netif_ap;
    esp_netif_t *esp_netif_sta;
    esp_netif_t *esp_netif_eth;
    void SetIpAddress(NetworkInterface interface, NetworkIpAddress ipConfig);
    void CreateSocket(uint16_t port);

public:
    TcpService(/* args */);
    void TcpAppStack();
    void SendTcpMessage(string message);

    void (*logString)(string TAG, string message);
    void (*logDword)(string TAG, int32_t logNumber);
    void (*logFloat)(string TAG, double logFloating);
    void (*RestartSystem)(void);
    void (*SendWifiApRecordsScanned)(void);
    void (*SetDefaultMemoryValues)(void);
    void (*SendDeviceInfo)(void);

    ~TcpService();
};

#endif