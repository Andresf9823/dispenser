#ifndef _FILE_SYSTEM_HPP_
#define _FILE_SYSTEM_HPP_

#include <GlobalDefines.hpp>
#include <MemoryKeys.hpp>
#include "nvs_flash.h"

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

    uint32_t ReadUint32tRecord(string key);
    bool WriteUint32tRecord(string key, uint32_t record);

    ~FileSystem();
};

#endif