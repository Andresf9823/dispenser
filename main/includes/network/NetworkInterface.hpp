#ifndef _NETWORK_INTERFACE_HPP_
#define _NETWORK_INTERFACE_HPP_

#include "../format/ArduinoJson-v6.21.3.h"
#include "Ip.hpp"
#include "protocols/Tcp.hpp"
#include "protocols/Http.hpp"

typedef struct _NetworkProperties
{
    bool dhcpEnabled;
    Ipv4 ip;
    string ssid;
    string password;
    uint8_t mac[6];
    uint8_t targetMac[6];
    uint8_t authentication;
} NetworkProperties;

class Network : public Tcp, Http
{
private:
    /* data */
public:
    Network() {}
    ~Network() {}
};

#endif