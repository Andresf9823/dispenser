#ifndef _I_NETWORK_PROTOCOL_HPP
#define _I_NETWORK_PROTOCOL_HPP

#include "GlobalDefines.hpp"

class INetworkProtocol
{
public:
    INetworkProtocol(){}
    virtual void createClient(uint16_t _port) = 0;
    virtual void createServer(uint16_t _port) = 0;
    virtual void sendMessage(uint16_t _port, string message) = 0;
    ~INetworkProtocol(){}
};

#endif