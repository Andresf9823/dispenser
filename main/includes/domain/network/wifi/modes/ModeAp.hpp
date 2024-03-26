#ifndef _WIFI_MODE_AP_HPP_
#define _WIFI_MODE_AP_HPP_

#include "IWifiMode.hpp"

class IWifiModeAp : public IWifiMode
{

private:
    static constexpr string tag = "Wifi mode";
    const uint8_t index = 0;
public:
    IWifiModeAp();
    void setConfiguration(vector<WifiConfiguration>  config) override;
    void getConfiguration() override;
    void setIpAddress(WifiConfiguration config) override;
    ~IWifiModeAp();
};

#endif