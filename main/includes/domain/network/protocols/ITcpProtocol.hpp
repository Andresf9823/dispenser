#ifndef _TCP_PROTOCOL_I_HPP
#define _TCP_PROTOCOL_I_HPP


#include <GlobalDefines.hpp>
#include <freertos/FreeRTOS.h>
#include "freertos/event_groups.h"
#include <freertos/task.h>
#include <freertos/timers.h>

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include <esp_log.h>

#include "INetworkProtocol.hpp"

#define TCP_RX_BUFFER_SIZE (KB) / 2
#define TCP_TX_BUFFER_SIZE (KB) * (1.5)
#define TCP_TASK_SIZE (KB) * (4)

// static int socketState;

class TcpProtocol : public INetworkProtocol
{

private:
    static void serverLaunch(void *pvParameters);
    static void serverTask(const int sock);
    static bool isValidFrame(char *buffer, uint len);

public:
    TcpProtocol();
    void createClient(uint16_t _port) override;
    void createServer(uint16_t _port) override;
    void sendMessage(uint16_t _port, string message) override;
    ~TcpProtocol();
};

#endif