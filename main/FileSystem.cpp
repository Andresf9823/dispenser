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
    uint8_t mac[6] = {DEFAULT_WIFI_MAC_0, DEFAULT_WIFI_MAC_1, DEFAULT_WIFI_MAC_2, DEFAULT_WIFI_MAC_3, DEFAULT_WIFI_MAC_4, DEFAULT_WIFI_MAC_5};
    uint8_t ipAddress[4] = {192, 168, 0, 1};
    uint8_t subnet[4] = {255, 255, 255, 0};
    uint8_t gateway[4] = {192, 168, 0, 1};
    Formatter format;

    this->WriteBooleanRecord(NVS_UART2_EN, true);
    ESP_LOGI(tag.c_str(), "%s", "WRITING INFO");

    this->WriteUint32tRecord(NVS_DEVICE_ID, 1234091869);
    this->WriteUint8tRecord(NVS_WIFI_MODE, WiFiMode::Ap);

    this->WriteStringRecord(NVS_AP_SSID, "BERDUGO_ESP");
    this->WriteStringRecord(NVS_AP_PASSWORD, "123456789");
    this->WriteUint8tRecord(NVS_AP_AUTH_MODE, wifi_auth_mode_t::WIFI_AUTH_WPA2_PSK);
    this->WriteStringRecord(NVS_AP_MAC, format.macToString(mac, sizeof(mac)));
    this->WriteStringRecord(NVS_AP_IP_ADDRESS, format.ipAddressToString(ipAddress));
    this->WriteStringRecord(NVS_AP_SUBNET, format.ipAddressToString(subnet));
    this->WriteStringRecord(NVS_AP_GATEWAY, format.ipAddressToString(gateway));
    this->WriteBooleanRecord(NVS_AP_DHCP_ENABLE, true);

    this->WriteStringRecord(NVS_STA_SSID, "");
    this->WriteStringRecord(NVS_STA_PASSWORD, "");
    this->WriteUint8tRecord(NVS_STA_AUTH_MODE, wifi_auth_mode_t::WIFI_AUTH_OPEN);
    this->WriteStringRecord(NVS_STA_TARGET_MAC, "");
    this->WriteStringRecord(NVS_STA_MAC, "");
    this->WriteStringRecord(NVS_STA_IP_ADDRESS, "");
    this->WriteStringRecord(NVS_STA_SUBNET, "");
    this->WriteStringRecord(NVS_STA_GATEWAY, "");
    this->WriteBooleanRecord(NVS_STA_DHCP_ENABLE, true);
    this->WriteStringRecord(NVS_STA_API_HOST, "");

    ESP_LOGI(tag.c_str(), "%s", "FINSIHED");
}

WifiConfig FileSystem::ReadWifiConfig()
{
    Formatter format;
    WifiConfig config;
    uint8_t ip[4];
    uint8_t mac[6];
    uint8_t macSize = sizeof(mac);
    uint8_t ipSize = sizeof(ip);

    memset(config.ApConfig.mac, 0, sizeof(config.ApConfig.mac));
    memset(config.ApConfig.ip, 0, sizeof(config.ApConfig.ip));
    memset(config.ApConfig.mask, 0, sizeof(config.ApConfig.mask));
    memset(config.ApConfig.gateway, 0, sizeof(config.ApConfig.gateway));

    memset(config.StaConfig.mac, 0, sizeof(config.StaConfig.mac));
    memset(config.StaConfig.ip, 0, sizeof(config.StaConfig.ip));
    memset(config.StaConfig.mask, 0, sizeof(config.StaConfig.mask));
    memset(config.StaConfig.gateway, 0, sizeof(config.StaConfig.gateway));

    ESP_LOGI(tag.c_str(), "%s", "Reading Wifi Configuration");
    config.mode = static_cast<WiFiMode>(this->ReadUint8tRecord(NVS_WIFI_MODE));

    ESP_LOGI(tag.c_str(), "%s", "Reading Ap Configuration");
    config.ApConfig.auth = static_cast<WiFiMode>(this->ReadUint8tRecord(NVS_AP_AUTH_MODE));
    config.ApConfig.ssid = this->ReadStringRecord(NVS_AP_SSID);
    config.ApConfig.password = this->ReadStringRecord(NVS_AP_PASSWORD);
    config.ApConfig.dhcpEnlabled = this->ReadBooleanRecord(NVS_AP_DHCP_ENABLE);

    ESP_LOGI(tag.c_str(), "%s", "Reading AP ipAddress");
    format.stringToMac(mac, this->ReadStringRecord(NVS_AP_MAC));
    memcpy(config.ApConfig.mac, mac, macSize);
    format.stringToIpAddress(ip, this->ReadStringRecord(NVS_AP_IP_ADDRESS));
    memcpy(config.ApConfig.ip, ip, ipSize);
    format.stringToIpAddress(ip, this->ReadStringRecord(NVS_AP_SUBNET));
    memcpy(config.ApConfig.mask, ip, ipSize);
    format.stringToIpAddress(ip, this->ReadStringRecord(NVS_AP_GATEWAY));
    memcpy(config.ApConfig.gateway, ip, ipSize);

    ESP_LOGI(tag.c_str(), "%s", "Reading Sta Configuration");
    config.StaConfig.auth = static_cast<WiFiMode>(this->ReadUint8tRecord(NVS_STA_AUTH_MODE));
    config.StaConfig.ssid = this->ReadStringRecord(NVS_STA_SSID);
    config.StaConfig.password = this->ReadStringRecord(NVS_STA_PASSWORD);
    config.StaConfig.dhcpEnlabled = this->ReadBooleanRecord(NVS_STA_DHCP_ENABLE);

    ESP_LOGI(tag.c_str(), "%s", "Reading Sta ipAddress");
    format.stringToMac(mac, this->ReadStringRecord(NVS_STA_MAC));
    memcpy(config.StaConfig.mac, mac, macSize);
    format.stringToMac(mac, this->ReadStringRecord(NVS_STA_TARGET_MAC));
    memcpy(config.StaConfig.apMac, mac, macSize);
    format.stringToIpAddress(ip, this->ReadStringRecord(NVS_STA_IP_ADDRESS));
    memcpy(config.StaConfig.ip, ip, ipSize);
    format.stringToIpAddress(ip, this->ReadStringRecord(NVS_STA_SUBNET));
    memcpy(config.StaConfig.mask, ip, ipSize);
    format.stringToIpAddress(ip, this->ReadStringRecord(NVS_STA_GATEWAY));
    memcpy(config.StaConfig.gateway, ip, ipSize);

    ESP_LOGI(tag.c_str(), "%s", "Finish Wifi Configuration");
    return config;
}

ApiConfig FileSystem::GetApiConfig()
{
    ApiConfig config;
    ESP_LOGI(tag.c_str(), "%s", "Reading Sta Api configuration");
    config.host = this->ReadStringRecord(NVS_STA_API_HOST);
    return config;
}

string FileSystem::ReadStringRecord(string _key)
{
    const char *key = _key.c_str();
    char record[50];
    size_t _size = sizeof(record);
    memset(record, 0, _size);
    if (this->Open(_key, NVS_READONLY))
        this->ErrorCheck(nvs_get_str(handle, key, record, &_size), key);
    nvs_close(handle);
    return string(record);
}

bool FileSystem::WriteStringRecord(string _key, string record)
{
    bool writed = false;
    const char *key = _key.c_str();
    if (this->Open(_key, NVS_READWRITE))
    {
        writed = this->ErrorCheck(nvs_set_str(handle, key, record.c_str()), _key);
        nvs_commit(handle);
        nvs_close(handle);
        return writed;
    }
    ESP_LOGE(tag.c_str(), "Failed writing  string: '%s'", key);
    return false;
}

bool FileSystem::ReadBooleanRecord(string _key)
{
    return this->ReadUint8tRecord(_key) == 0 ? false : true;
}

bool FileSystem::WriteBooleanRecord(string _key, bool record)
{
    return this->WriteUint8tRecord(_key, record);
}

uint8_t FileSystem::ReadUint8tRecord(string _key)
{
    uint8_t value;
    const char *key = _key.c_str();
    if (this->Open(_key, NVS_READONLY))
        this->ErrorCheck(nvs_get_u8(handle, key, &value), key);
    nvs_close(handle);
    return value;
}

bool FileSystem::WriteUint8tRecord(string _key, uint8_t record)
{
    bool writed = false;
    const char *key = _key.c_str();
    if (this->Open(_key, NVS_READWRITE))
    {
        writed = this->ErrorCheck(nvs_set_u8(handle, key, record), _key);
        nvs_commit(handle);
        nvs_close(handle);
        return writed;
    }
    ESP_LOGE(tag.c_str(), "Failed writing  uint32: '%s'", key);
    return false;
}

uint32_t FileSystem::ReadUint32tRecord(string _key)
{
    uint32_t value;
    const char *key = _key.c_str();
    if (this->Open(_key, NVS_READONLY))
        this->ErrorCheck(nvs_get_u32(handle, key, &value), key);
    nvs_close(handle);
    return value;
}

bool FileSystem::WriteUint32tRecord(string _key, uint32_t record)
{
    bool writed = false;
    const char *key = _key.c_str();
    if (this->Open(_key, NVS_READWRITE))
    {
        writed = this->ErrorCheck(nvs_set_u32(handle, key, record), _key);
        nvs_commit(handle);
        nvs_close(handle);
        return writed;
    }
    ESP_LOGE(tag.c_str(), "Failed writing  uint32: '%s'", key);
    return false;
}

bool FileSystem::Open(string key, nvs_open_mode_t openMode)
{
    if (this->ErrorCheck(nvs_open(key.c_str(), openMode, &handle), key))
        return true;
    ESP_LOGE(tag.c_str(), "Failed at Open: '%s' ", key.c_str());
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
        ESP_LOGE(tag.c_str(), "The value '%s' is not initialized yet!", key);
        break;
    case ESP_ERR_NVS_KEY_TOO_LONG:
        ESP_LOGE(tag.c_str(), "The key '%s' is too long!", key);
        break;
    case ESP_ERR_NVS_PART_NOT_FOUND:
        ESP_LOGE(tag.c_str(), "The value for key '%s' not found", key);
        break;
    case ESP_ERR_NVS_KEYS_NOT_INITIALIZED:
        ESP_LOGE(tag.c_str(), "The key '%s' is not initialized", key);
        break;
    case ESP_ERR_NVS_NOT_INITIALIZED:
        ESP_LOGE(tag.c_str(), "The storage driver of the key '%s' is not initialized", key);
        break;
    case ESP_ERR_NVS_INVALID_NAME:
        ESP_LOGE(tag.c_str(), "The key's string value  '%s' is invalid, size must be less than 15, size: %d", key, strlen(key));
        break;
    case ESP_ERR_NO_MEM:
        ESP_LOGE(tag.c_str(), "Memory could not be allocated for key's string value  '%s' ", key);
        break;
    case ESP_ERR_NVS_NOT_ENOUGH_SPACE:
        ESP_LOGE(tag.c_str(), "no space for the new entry key '%s'  or there are too many different namespaces (maximum allowed different namespaces: 254)", key);
        break;
    case ESP_ERR_INVALID_ARG:
        ESP_LOGE(tag.c_str(), "In key '%s' out_handle is equal to NULL", key);
        break;
    default:
        ESP_LOGE(tag.c_str(), "Error reading String '%s' (%s) !", key, esp_err_to_name(err));
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