#ifndef _WIFI_HPP_
#define _WIFI_HPP_

#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_wifi.h>
#include <GlobalDefines.hpp>
#include "modes/ModeAp.hpp"
#include "modes/ModeSta.hpp"
#include "modes/ModeApSta.hpp"
#include "../../database/localstorage/LocalStorage.hpp"

class Wifi : IWifiModeAp, IWifiModeSta, IWifiModeApSta
{
private:
    static constexpr string tag = "Wifi";
    vector<WifiConfiguration> configuration;
    static void WifiEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

public:
    Wifi();
    void init(IWifiMode *mode);
    void (*logString)(string TAG, string message);
    void (*logDword)(string TAG, int64_t logNumber);
    void (*logFloat)(string TAG, double logFloating);
    ~Wifi();
};

#endif