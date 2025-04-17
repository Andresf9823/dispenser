#ifndef STORAGE_REPOSITORY_HPP
#define STORAGE_REPOSITORY_HPP

#include <string>
#include <vector>
#include "../network/wifi/Wifi.hpp"
#include "../network/protocols/Http.hpp"

class StorageRepository {
public:
    virtual ~StorageRepository() = default;

    virtual void setDefaultValues() = 0;
    virtual void loadAllConfigs() = 0;
    virtual void saveAllConfigs() = 0;

    virtual void loadDeviceConfig() = 0;
    virtual void saveDeviceConfig() = 0;

    virtual void loadAllNetworksConfig() = 0;
    virtual void saveAllNetworksConfig() = 0;

    virtual void saveStationTarget(ApRecordList record, std::string password) = 0;
    virtual ApRecordList loadStationTarget() = 0;
    
    virtual void saveWifiConfig(WifiConfig config) = 0;
    virtual WifiConfig loadWifiConfig() = 0;

    virtual void loadEthernetConfig() = 0;
    virtual void saveEthernetConfig() = 0;

    virtual void saveApiConfig(ApiConfig config) = 0;
    virtual ApiConfig readApiConfig() = 0;

    virtual void loadAllIOConfig() = 0;
    virtual void saveAllIOConfig() = 0;

    virtual void loadUartsConfig() = 0;
    virtual void saveUartsConfig() = 0;

    virtual void loadSpiConfig() = 0;
    virtual void saveSpiConfig() = 0;

    virtual void loadI2cConfig() = 0;
    virtual void saveI2cConfig() = 0;

    virtual void loadGpioConfig() = 0;
    virtual void saveGpioConfig() = 0;   
    
    
};

#endif // STORAGE_REPOSITORY_HPP

