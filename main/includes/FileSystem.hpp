#ifndef _FILE_SYSTEM_HPP_
#define _FILE_SYSTEM_HPP_

#include <MemoryKeys.hpp>
#include <nvs_flash.h>
#include <Formatter.hpp>

using namespace std;

class FileSystem
{
private:
    static constexpr string tag = "FILE SYSTEM";
    nvs_sec_cfg_t partition;
    nvs_handle_t handle;
    bool Open(string key, nvs_open_mode_t mode);
    bool ErrorCheck(esp_err_t err, string key);

public:
    FileSystem();
    void FormatPartition();
    void SetDefaultValues();

    string ReadStringRecord(string _key);
    bool ReadBooleanRecord(string _key);
    uint8_t ReadUint8tRecord(string key);
    uint32_t ReadUint32tRecord(string key);
    WifiConfig ReadWifiConfig();

    bool WriteStringRecord(string _key, string record);
    bool WriteBooleanRecord(string _key, bool record);
    bool WriteUint32tRecord(string key, uint32_t record);
    bool WriteUint8tRecord(string key, uint8_t record);

    ~FileSystem();
};

#endif