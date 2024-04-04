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
    sendDeviceInfo = 0x0B,
    sendWifiApRecords = 0x1B,
    saveWifiApRecord = 0x2B,
    setDefaultMemoryValues = 0x0C
} ProtocolCommand;

typedef struct _TcpServerConfiguration
{
    uint16_t port;
    void (*callback)(char *bufferIn);
} TcpServerConfiguration;

/*TcpSocker buffer and state flag*/
// static char tcpBuffer[TCP_RX_BUFFER_SIZE];
static int socketState;

class Tcp : public ProtocolsInterface
{
private:
    static constexpr string tag = "TCP";
    static bool isValidFrame(char *frame, uint len);
    static void serverLaunch(void *pvParameters);
    static void serverTask(const int sock,  void (*tcpBuffer)(char *));
    uint16_t _port[TCP_MAX_SERVERS];
    char *_buffer[TCP_MAX_SERVERS][TCP_RX_BUFFER_SIZE];
    uint16_t _quantityServersOn;

protected:
    esp_netif_t *esp_netif_ap;
    esp_netif_t *esp_netif_sta;
    esp_netif_t *esp_netif_eth;

public:
    Tcp(/* args */);
    // void tcpAppStack(char * buffer);
    void createTcpServer(TcpServerConfiguration &config);
    void sendTcpMessage(string message);
    void (*RestartSystem)(void);
    void (*SendWifiApRecordsScanned)(void);
    void (*SaveWifiApRecord)(void);
    void (*SetDefaultMemoryValues)(void);
    void (*SendDeviceInfo)(void);

    ~Tcp();
};
#endif