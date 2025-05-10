#ifndef _NETWORK_INTERFACE_HPP_
#define _NETWORK_INTERFACE_HPP_

#include "../format/ArduinoJson-v6.21.3.h"
#include "protocols/protocols/TcpServer.hpp"
#include "protocols/protocols/TcpClient.hpp"

typedef struct _NetworkProperties
{
    bool dhcpEnabled;
    struct {
        uint8_t ip[4];
        uint8_t mask[4];
        uint8_t gateway[4];
    } ip;
    string ssid;
    string password;
    uint8_t mac[6];
    uint8_t targetMac[6];
    uint8_t authentication;
} NetworkProperties;

class Network : public TcpServer
{
private:
    static constexpr string tag = "NETWORK";
public:
    ~Network() =  default;
};

#endif