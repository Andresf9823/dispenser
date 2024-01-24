#include <FileSystem.hpp>

FileSystem::FileSystem(/* args */)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_LOGI(tag.c_str(), "%s", "Flash init failed, launch again");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    else if (err == ESP_OK)
        ESP_LOGI(tag.c_str(), "%s", "Flash init done");

    ESP_ERROR_CHECK(err);
}

void FileSystem::SetDefaultValues()
{
    this->FormatPartition();
    this->WriteUint32tRecord(UART2_EN, 2);
    this->ReadUint32tRecord(UART2_EN);
}

uint32_t FileSystem::ReadUint32tRecord(string _key)
{
    uint32_t value;
    const char *key = _key.c_str();
    if (this->Open(_key, NVS_READONLY))
        this->ErrorCheck(nvs_get_u32(handle, key, &value), key);
    nvs_close(handle);
    ESP_LOGE(tag.c_str(), "Value: '%ld'", value);
    return value;
}

bool FileSystem::WriteUint32tRecord(string _key, uint32_t record)
{
    bool readed = false;
    const char *key = _key.c_str();
    if (this->Open(_key, NVS_READWRITE))
    {
        uint32_t record = 0;
        ESP_LOGE(tag.c_str(), "Value:: '%d'", record);
        readed = this->ErrorCheck(nvs_set_u32(handle, key, record), _key);
        nvs_close(handle);
        return readed;
    }
    ESP_LOGI(tag.c_str(), "Failed at Write: '%s'", key);
    return false;
}

bool FileSystem::Open(string key, nvs_open_mode_t openMode)
{
    if (nvs_open(key.c_str(), openMode, &handle) == ESP_OK)
        return true;
    ESP_LOGI(tag.c_str(), "Failed at Open: '%s' ", key.c_str());
    return false;
}

bool FileSystem::ErrorCheck(esp_err_t err, string _key)
{
    const char *key = _key.c_str();
    switch (err)
    {
    case ESP_OK:
        return true;
        break;
    case ESP_ERR_NVS_NOT_FOUND:
        ESP_LOGI(tag.c_str(), "The value '%s' is not initialized yet!", key);
        break;
    case ESP_ERR_NVS_KEY_TOO_LONG:
        ESP_LOGI(tag.c_str(), "The key '%s' is too long!", key);
        break;
    case ESP_ERR_NVS_PART_NOT_FOUND:
        ESP_LOGI(tag.c_str(), "The value for key '%s' not found", key);
        break;
    case ESP_ERR_NVS_KEYS_NOT_INITIALIZED:
        ESP_LOGI(tag.c_str(), "The key '%s' is not initialized", key);
        break;
    case ESP_ERR_NVS_NOT_INITIALIZED:
        ESP_LOGI(tag.c_str(), "The storage driver of the key '%s' is not initialized", key);
        break;
    case ESP_ERR_NVS_INVALID_NAME:
        ESP_LOGI(tag.c_str(), "The key's string value  '%s' is invalid", key);
        break;
    case ESP_ERR_NO_MEM:
        ESP_LOGI(tag.c_str(), "Memory could not be allocated for key's string value  '%s' ", key);
        break;
    case ESP_ERR_NVS_NOT_ENOUGH_SPACE:
        ESP_LOGI(tag.c_str(), "no space for the new entry key '%s'  or there are too many different namespaces (maximum allowed different namespaces: 254)", key);
        break;
    case ESP_ERR_INVALID_ARG:
        ESP_LOGI(tag.c_str(), "In key '%s' out_handle is equal to NULL", key);
        break;
    default:
        ESP_LOGI(tag.c_str(), "Error reading String '%s' (%s) !", key, esp_err_to_name(err));
        break;
    }
    return false;
}

void FileSystem::FormatPartition()
{
    ESP_LOGI(tag.c_str(), "%s", "Fomatting local memory");
    nvs_erase_all(handle);
}

FileSystem::~FileSystem()
{
}