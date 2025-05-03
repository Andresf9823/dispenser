#ifndef _ETHERNET_HPP_
#define _ETHERNET_HPP_


#include <GlobalDefines.hpp>
#include "../NetworkInterface.hpp"


class Ethernet : public TcpServer
{
private:

public:
    Ethernet();
    ~Ethernet();
};
#endif