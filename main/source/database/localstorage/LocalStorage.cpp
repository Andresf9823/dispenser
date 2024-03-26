#include <domain/database/localstorage/LocalStorage.hpp>

LocalStorage::LocalStorage(/* args */)
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

void LocalStorage::setDefaultValues()
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
    // this->WriteUint8tRecord(NVS_WIFI_MODE, WiFiMode::ApStation);

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
    uint8_t _mac[6] = {0x10, 0x33, 0xC2, 0x75, 0x43 , 0x10};
    this->WriteStringRecord(NVS_STA_MAC, format.macToString(_mac, sizeof(_mac)));
    this->WriteStringRecord(NVS_STA_IP_ADDRESS, "");
    this->WriteStringRecord(NVS_STA_SUBNET, "");
    this->WriteStringRecord(NVS_STA_GATEWAY, "");
    this->WriteBooleanRecord(NVS_STA_DHCP_ENABLE, true);
    this->WriteStringRecord(NVS_STA_TARGET_MAC, "");
    
    this->WriteStringRecord(NVS_STA_API_HOST, "");

    ESP_LOGI(tag.c_str(), "%s", "FINSIHED");
}

string LocalStorage::ReadStringRecord(string _key)
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

bool LocalStorage::WriteStringRecord(string _key, string record)
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

bool LocalStorage::ReadBooleanRecord(string _key)
{
    return this->ReadUint8tRecord(_key) == 0 ? false : true;
}

bool LocalStorage::WriteBooleanRecord(string _key, bool record)
{
    return this->WriteUint8tRecord(_key, record);
}

uint8_t LocalStorage::ReadUint8tRecord(string _key)
{
    uint8_t value;
    const char *key = _key.c_str();
    if (this->Open(_key, NVS_READONLY))
        this->ErrorCheck(nvs_get_u8(handle, key, &value), key);
    nvs_close(handle);
    return value;
}

bool LocalStorage::WriteUint8tRecord(string _key, uint8_t record)
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

uint32_t LocalStorage::ReadUint32tRecord(string _key)
{
    uint32_t value;
    const char *key = _key.c_str();
    if (this->Open(_key, NVS_READONLY))
        this->ErrorCheck(nvs_get_u32(handle, key, &value), key);
    nvs_close(handle);
    return value;
}

bool LocalStorage::WriteUint32tRecord(string _key, uint32_t record)
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

void LocalStorage::readWifiConfiguration(vector<WifiConfiguration> *config)
{
    config->clear();
    Formatter format;
    WifiConfiguration modeConfig;
    uint8_t ip[4];
    uint8_t mac[6];
    uint8_t macSize = sizeof(mac);
    uint8_t ipSize = sizeof(ip);

    memset(modeConfig.mac, 0, sizeof(modeConfig.mac));
    memset(modeConfig.ipv4.ip, 0, sizeof(modeConfig.ipv4.ip));
    memset(modeConfig.ipv4.mask, 0, sizeof(modeConfig.ipv4.mask));
    memset(modeConfig.ipv4.gateway, 0, sizeof(modeConfig.ipv4.gateway));

    ESP_LOGI(tag.c_str(), "%s", "Reading Wifi Configuration");

    ESP_LOGI(tag.c_str(), "%s", "Reading Ap Configuration");
    // modeConfig.authentication = static_cast<WiFiMode>(this->ReadUint8tRecord(NVS_AP_AUTH_MODE));
    modeConfig.authentication = 0;
    modeConfig.ssid = this->ReadStringRecord(NVS_AP_SSID);
    modeConfig.password = this->ReadStringRecord(NVS_AP_PASSWORD);
    modeConfig.dhcpEnabled = this->ReadBooleanRecord(NVS_AP_DHCP_ENABLE);

    format.stringToMac(mac, this->ReadStringRecord(NVS_AP_MAC));
    memcpy(modeConfig.mac, mac, macSize);
    format.stringToIpAddress(ip, this->ReadStringRecord(NVS_AP_IP_ADDRESS));
    memcpy(modeConfig.ipv4.ip, ip, ipSize);
    format.stringToIpAddress(ip, this->ReadStringRecord(NVS_AP_SUBNET));
    memcpy(modeConfig.ipv4.mask, ip, ipSize);
    format.stringToIpAddress(ip, this->ReadStringRecord(NVS_AP_GATEWAY));
    memcpy(modeConfig.ipv4.gateway, ip, ipSize);

    config->push_back(modeConfig);

    ESP_LOGI(tag.c_str(), "%s", "Reading Sta Configuration");
    // modeConfig.authentication = static_cast<WiFiMode>(this->ReadUint8tRecord(NVS_STA_AUTH_MODE));
    modeConfig.authentication =  2;
    modeConfig.ssid = this->ReadStringRecord(NVS_STA_SSID);
    modeConfig.password = this->ReadStringRecord(NVS_STA_PASSWORD);
    modeConfig.dhcpEnabled = this->ReadBooleanRecord(NVS_STA_DHCP_ENABLE);

    format.stringToMac(mac, this->ReadStringRecord(NVS_STA_MAC));
    memcpy(modeConfig.mac, mac, macSize);
    format.stringToIpAddress(ip, this->ReadStringRecord(NVS_STA_IP_ADDRESS));
    memcpy(modeConfig.ipv4.ip, ip, ipSize);
    format.stringToIpAddress(ip, this->ReadStringRecord(NVS_STA_SUBNET));
    memcpy(modeConfig.ipv4.mask, ip, ipSize);
    format.stringToIpAddress(ip, this->ReadStringRecord(NVS_STA_GATEWAY));
    memcpy(modeConfig.ipv4.gateway, ip, ipSize);

    config->push_back(modeConfig);

    ESP_LOGI(tag.c_str(), "%s", "Finish Wifi Configuration");
}

bool LocalStorage::Open(string key, nvs_open_mode_t openMode)
{
    if (this->ErrorCheck(nvs_open(key.c_str(), openMode, &handle), key))
        return true;
    ESP_LOGE(tag.c_str(), "Failed at Open: '%s' ", key.c_str());
    return false;
}

bool LocalStorage::ErrorCheck(esp_err_t err, string _key)
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

void LocalStorage::FormatPartition()
{
    ESP_LOGI(tag.c_str(), "%s", "Fomatting local memory");
    nvs_erase_all(handle);
}

LocalStorage::~LocalStorage()
{
}