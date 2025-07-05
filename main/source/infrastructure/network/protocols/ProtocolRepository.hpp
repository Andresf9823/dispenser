#ifndef _PROTOCOLS_INTERFACE_HPP_
#define _PROTOCOLS_INTERFACE_HPP_

#include "GlobalDefines.hpp"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

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

enum class IpServerType
{
    TCP = 1,
    UDP = 2,
};

typedef struct _IpServerConfiguration
{
    uint16_t port;
    IpServerType type;
    function<string(char *dataToSend)> callback;
} IpServerConfiguration;

typedef struct _IpSclientConfiguration
{
    uint16_t port;
    IpServerType type;
    uint8_t ipTarget[4];
    string (*callback)(char *dataToSend);
} IpClientConfiguration;

class IpServerRepository
{
public:
    virtual void createServer(IpServerConfiguration &config) = 0;
};

class IpClientRepository
{
public:
    virtual void connectToServer(IpClientConfiguration &config) = 0;
};

#endif