#ifndef _WIFI_MODE_STA_HPP_
#define _WIFI_MODE_STA_HPP_

#include "../../TcpIp.hpp"
#include "domain/network/wifi/modes/IWifiMode.hpp"

#define MAXIMUM_SIZE_OF_SCAN_LIST 10

typedef struct _ApRecordList
{
    uint8_t mac[6];
    uint8_t ssid[6];
    uint8_t rssi;
    uint8_t authMode;
} ApRecordList;

class IWifiModeSta : public IWifiMode
{
private:
    static constexpr string tag = "Mode Station";
    const uint8_t index = 1;

public:
    IWifiModeSta();
    void setConfiguration(vector<WifiConfiguration> config) override;
    void getConfiguration() override;
    void setIpAddress(WifiConfiguration config) override;
    uint16_t scanWifiNetworks(ApRecordList *apRecords);
    ~IWifiModeSta();
};

#endif