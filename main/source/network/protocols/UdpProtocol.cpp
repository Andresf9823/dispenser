#include "domain/network/protocols/IUdpProtocol.hpp"

UdpProtocol::UdpProtocol()
{
}

void UdpProtocol::createClient(uint16_t _port) {}
void UdpProtocol::createServer(uint16_t _port) {}
void UdpProtocol::sendMessage(uint16_t _port, string message) {}

UdpProtocol::~UdpProtocol()
{
}