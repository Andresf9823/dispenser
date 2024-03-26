#ifndef _WIFI_MODE_AP_STA_HPP_
#define _WIFI_MODE_AP_STA_HPP_

#include "ModeAp.hpp"
#include "ModeSta.hpp"

class IWifiModeApSta : public IWifiMode
{
public:
    IWifiModeApSta();
    void setConfiguration(vector<WifiConfiguration>  config) override;
    void getConfiguration() override;
    void setIpAddress(WifiConfiguration config) override;
    ~IWifiModeApSta();
};

#endif
