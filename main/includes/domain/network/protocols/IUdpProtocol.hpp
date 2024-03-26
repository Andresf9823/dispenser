#ifndef _UDP_PROTOCOL_HPP
#define _UDP_PROTOCOL_HPP_

#include "INetworkProtocol.hpp"

class UdpProtocol : public INetworkProtocol
{
public:
    UdpProtocol();
    void createClient(uint16_t _port) override;
    void createServer(uint16_t _port) override;
    void sendMessage(uint16_t _port, string message) override;
    ~UdpProtocol();
};

#endif