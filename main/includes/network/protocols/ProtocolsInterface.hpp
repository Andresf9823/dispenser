#ifndef _PROTOCOLS_INTERFACE_HPP_
#define _PROTOCOLS_INTERFACE_HPP_

#include "GlobalDefines.hpp"

class ProtocolsInterface
{
public:
    ProtocolsInterface() {}
    void (*logString)(string TAG, string message);
    void (*logDword)(string TAG, int64_t logNumber);
    void (*logFloat)(string TAG, double logFloating);
    ~ProtocolsInterface() {}
};

#endif