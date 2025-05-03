#ifndef _PROTOCOLS_INTERFACE_HPP_
#define _PROTOCOLS_INTERFACE_HPP_

#include "GlobalDefines.hpp"

typedef enum protocolCommand
{
    restartSystem = 0x00,
    login = 0x01,
    sendDeviceInfo = 0x0B,
    sendWifiApRecords = 0x1B,
    saveWifiApRecord = 0x2B,
    setWifiMode = 0x3B,
    setDefaultMemoryValues = 0x0C,
    getDeviceConfiguration = 0x1C,
    setMac = 0x2C
} ProtocolCommand;

typedef struct _IpServerConfiguration
{
    uint16_t port;
    string (*callback)(char *dataToSend);
} IpServerConfiguration;

class IpServerRepository
{
public:
    virtual void createServer(IpServerConfiguration &config) = 0;
};

#endif