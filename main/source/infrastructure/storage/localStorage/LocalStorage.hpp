#ifndef _LOCAL_STORAGE_HPP_
#define _LOCAL_STORAGE_HPP_

#include <nvs_flash.h>
#include "../StorageRepository.hpp"
#include "../../format/Formatter.hpp"
#include "MemoryKeys.hpp"

using namespace std;

class LocalStorage : public StorageRepository
{
private:
    static constexpr string tag = "FILE SYSTEM";
    nvs_sec_cfg_t partition;
    nvs_handle_t handle;
    bool open(string key, nvs_open_mode_t mode);
    bool errorCheck(esp_err_t err, string key);
    NetworkProperties loadApConfig();
    NetworkProperties loadStaConfig();
    
    string readStringRecord(string _key);
    bool readBooleanRecord(string _key);
    uint8_t readByteRecord(string key);
    int64_t readDwordRecord(string key);

    bool writeStringRecord(string _key, string record);
    bool writeBooleanRecord(string _key, bool record);
    bool writeDwordRecord(string key, int64_t record);
    bool writeByteRecord(string key, uint8_t record);

public:
    LocalStorage();
    void formatPartition();
    
    void setDefaultValues() override;

    void loadAllConfigs() override{};
    void saveAllConfigs() override{};

    void loadDeviceConfig() override{};
    void saveDeviceConfig() override{};

    void loadAllNetworksConfig() override{};
    void saveAllNetworksConfig() override{};

    void saveStationTarget(ApRecordList record, std::string password) override;
    ApRecordList loadStationTarget() override;

    void saveWifiConfig(WifiConfig config) override{};
    WifiConfig loadWifiConfig() override;

    void loadEthernetConfig() override{};
    void saveEthernetConfig() override{};

    void saveApiConfig(ApiConfig config) override{};
    ApiConfig readApiConfig() override;

    void loadAllIOConfig() override{};
    void saveAllIOConfig() override{};

    void loadUartsConfig() override{};
    void saveUartsConfig() override{};

    void loadSpiConfig() override{};
    void saveSpiConfig() override{};

    void loadI2cConfig() override{};
    void saveI2cConfig() override{};

    void loadGpioConfig() override{};
    void saveGpioConfig() override{};   

    ApiConfig getApiConfig();

    ~LocalStorage() override;
};

#endif