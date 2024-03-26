#ifndef _LOCAL_STORAGE_HPP_
#define _LOCAL_STORAGE_HPP_

#include "MemoryKeys.hpp"
#include <nvs_flash.h>
#include "../Formatter.hpp"

using namespace std;

class LocalStorage
{
private:
    static constexpr string tag = "FILE SYSTEM";
    nvs_handle_t handle;
    bool Open(string key, nvs_open_mode_t mode);
    bool ErrorCheck(esp_err_t err, string key);

public:
    LocalStorage();
    void FormatPartition();
    void setDefaultValues();

    string ReadStringRecord(string _key);
    bool ReadBooleanRecord(string _key);
    uint8_t ReadUint8tRecord(string key);
    uint32_t ReadUint32tRecord(string key);
    void readWifiConfiguration(vector <WifiConfiguration> *config);
    bool WriteStringRecord(string _key, string record);
    bool WriteBooleanRecord(string _key, bool record);
    bool WriteUint32tRecord(string key, uint32_t record);
    bool WriteUint8tRecord(string key, uint8_t record);

    ~LocalStorage();
};

#endif