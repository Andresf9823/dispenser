#ifndef _WIFI_HPP_
#define _WIFI_HPP_

// #include <esp_event.h>
// #include <nvs_flash.h>
// #include <esp_wifi.h>
// #include <ArduinoJson-v6.21.3.h>
#include <GlobalDefines.hpp>
// #include "../INetwork.hpp"
#define MAXIMUM_SIZE_OF_SCAN_LIST 10

#define DEFAULT_WIFI_MAC_0 0x00
#define DEFAULT_WIFI_MAC_1 0x01
#define DEFAULT_WIFI_MAC_2 0x0A
#define DEFAULT_WIFI_MAC_3 0xA0
#define DEFAULT_WIFI_MAC_4 0x10
#define DEFAULT_WIFI_MAC_5 0x00

typedef struct _WifiEssentialConfiguration
{
    string ssid;
    string password;
    uint8_t rssid;
    uint8_t authentication;
    uint8_t mac[6];
} WifiEssentialConfiguration;

typedef struct _WifiConfiguration
{
    WifiEssentialConfiguration configuration;
    // Ip ip;
} WifiConfiguration;

class Wifi
{
private:
    static constexpr string tag = "Wifi Service";
    WifiConfiguration configuration;
    bool MacSafeValidator(uint8_t _defaultByte, uint8_t *_mac);

public:
    Wifi();
    void init();
    ~Wifi();
};

#endif