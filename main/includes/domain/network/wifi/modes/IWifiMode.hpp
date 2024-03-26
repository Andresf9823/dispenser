#ifndef _I_WIFI_MODE_HPP
#define _I_WIFI_MODE_HPP

#include "../../INetwork.hpp"
#include <esp_log.h>
#include <esp_wifi.h>

#define DEFAULT_WIFI_MAC_0 0x00
#define DEFAULT_WIFI_MAC_1 0x01
#define DEFAULT_WIFI_MAC_2 0x0A
#define DEFAULT_WIFI_MAC_3 0xA0
#define DEFAULT_WIFI_MAC_4 0x10
#define DEFAULT_WIFI_MAC_5 0x00

typedef struct _WifiConfiguration
{
    bool dhcpEnabled;
    uint8_t rssid;
    uint8_t authentication;
    uint8_t mac[6];
    string ssid;
    string password;
    Ip ipv4;
} WifiConfiguration;

class IWifiMode : public INetwork
{
protected:
    bool macSafeValidator(uint8_t _defaultByte, uint8_t *_mac)
    {
        if (_mac[0] == (char)0x01)
        {
            _mac[0] = DEFAULT_WIFI_MAC_0;
            _mac[1] = DEFAULT_WIFI_MAC_1;
            _mac[2] = DEFAULT_WIFI_MAC_2;
            _mac[3] = DEFAULT_WIFI_MAC_3;
            _mac[4] = DEFAULT_WIFI_MAC_4;
            _mac[5] = _defaultByte;
            return false;
        }
        return true;
    }

public:
    IWifiMode() {}
    virtual void getConfiguration() = 0;
    virtual void setConfiguration(vector<WifiConfiguration> config) = 0;
    virtual void setIpAddress(WifiConfiguration config) = 0;
    ~IWifiMode() {}
};

#endif