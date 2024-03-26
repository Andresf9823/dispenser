#ifndef _TCP_IP_HPP_
#define _TCP_IP_HPP_

#include <freertos/FreeRTOS.h>
#include "freertos/event_groups.h"
#include <freertos/task.h>
#include <freertos/timers.h>
#include <esp_wifi.h>

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>


#include "GlobalDefines.hpp"
#include "protocols/ITcpProtocol.hpp"
#include "protocols/IUdpProtocol.hpp"

#define TCP_RX_BUFFER 1024
#define TCP_TX_BUFFER 1024

typedef struct _Ip
{
    uint8_t ip[4];
    uint8_t mask[4];
    uint8_t gateway[4];
} Ip;

class TcpIp : public TcpProtocol, UdpProtocol
{
    private:
    static constexpr string tag = "TcpIp";
protected:
    Ip ip;
    bool dhcpEnable;

public:
    TcpIp() {}
    ~TcpIp() {}
};

#endif