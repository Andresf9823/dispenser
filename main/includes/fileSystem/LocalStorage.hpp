#ifndef _LOCAL_STORAGE_HPP_
#define _LOCAL_STORAGE_HPP_

#include "MemoryKeys.hpp"
#include <nvs_flash.h>
#include "../format/Formatter.hpp"

using namespace std;

class LocalStorage
{
private:
    static constexpr string tag = "FILE SYSTEM";
    nvs_sec_cfg_t partition;
    nvs_handle_t handle;
    bool open(string key, nvs_open_mode_t mode);
    bool errorCheck(esp_err_t err, string key);
    NetworkProperties readApConfig();
    NetworkProperties readStaConfig();

public:
    LocalStorage();
    void formatPartition();
    void setDefaultValues();

    string readStringRecord(string _key);
    bool readBooleanRecord(string _key);
    uint8_t readByteRecord(string key);
    int64_t readDwordRecord(string key);
    WifiConfig readWifiConfig();
    ApiConfig getApiConfig();
    void saveStationTarget(ApRecordList record, string password);

    bool writeStringRecord(string _key, string record);
    bool writeBooleanRecord(string _key, bool record);
    bool writeDwordRecord(string key, int64_t record);
    bool writeByteRecord(string key, uint8_t record);

    ~LocalStorage();
};

#endif