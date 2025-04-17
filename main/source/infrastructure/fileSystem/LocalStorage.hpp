#ifndef LOCAL_STORAGE_HPP
#define LOCAL_STORAGE_HPP

#include "StorageRepository.hpp"

class LocalStorage : public StorageRepository {
public:
    LocalStorage();
    ~LocalStorage() override;

    void setDefaultValues() override;
    void loadAllConfigs() override;
    void saveAllConfigs() override;
    void loadDeviceConfig() override;
    void saveDeviceConfig() override;
    void loadAllNetworksConfig() override;
    void saveAllNetworksConfig() override;
    void saveStationTarget(ApRecordList record, std::string password) override;
    void saveWifiConfig(WifiConfig config) override;
    WifiConfig loadWifiConfig() override;
    void loadEthernetConfig() override;
    void saveEthernetConfig() override;
    void saveApiConfig(ApiConfig config) override;
    ApiConfig readApiConfig() override;
    void loadAllIOConfig() override;
    void saveAllIOConfig() override;
    void loadUartsConfig() override;
    void saveUartsConfig() override;
    void loadSpiConfig() override;
    void saveSpiConfig() override;
    void loadI2cConfig() override;
    void saveI2cConfig() override;
    void loadGpioConfig() override;
    void saveGpioConfig() override;
};

#endif // LOCAL_STORAGE_HPP 