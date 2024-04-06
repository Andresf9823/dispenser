#ifndef _TCP_HPP_
#define _TCP_HPP_

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

#include "ProtocolsInterface.hpp"

using namespace std;

#define TCP_RX_BUFFER_SIZE (KB) / 2
#define TCP_TX_BUFFER_SIZE (KB) * (1.5)
#define TCP_TASK_SIZE (KB) * (6)
#define TCP_MAX_SERVERS 5

typedef enum protocolCommand
{
    restartSystem = 0x00,
    login = 0x01,
    sendDeviceInfo = 0x0B,
    sendWifiApRecords = 0x1B,
    saveWifiApRecord = 0x2B,
    setDefaultMemoryValues = 0x0C,
    setMac = 0x1C
} ProtocolCommand;

typedef struct _TcpServerConfiguration
{
    uint16_t port;
    string (*callback)(char *bufferIn);
} TcpServerConfiguration;

class Tcp : public ProtocolsInterface
{
private:
    static constexpr string tag = "TCP";
    static void serverLaunch(void *pvParameters);
    static bool isValidFrame(char *frame, uint len);
    static void sendTcpMessage(string message, int &socketState);
    static void serverTask(const int sock,  string (*tcpBuffer)(char *));
    char *_buffer[TCP_MAX_SERVERS][TCP_RX_BUFFER_SIZE];
    uint16_t _port[TCP_MAX_SERVERS];
    uint16_t _quantityServersOn;

protected:
    esp_netif_t *esp_netif_ap;
    esp_netif_t *esp_netif_sta;
    esp_netif_t *esp_netif_eth;

public:
    Tcp(/* args */);
    void createTcpServer(TcpServerConfiguration &config);

    ~Tcp();
};
#endif