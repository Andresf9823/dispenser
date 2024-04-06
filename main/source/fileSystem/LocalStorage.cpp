#include "fileSystem/LocalStorage.hpp"

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
    this->formatPartition();
    uint8_t mac[6] = {DEFAULT_WIFI_MAC_0, DEFAULT_WIFI_MAC_1, DEFAULT_WIFI_MAC_2, DEFAULT_WIFI_MAC_3, DEFAULT_WIFI_MAC_4, DEFAULT_WIFI_MAC_5};
    uint8_t ipAddress[4] = {192, 168, 0, 1};
    uint8_t subnet[4] = {255, 255, 255, 0};
    uint8_t gateway[4] = {192, 168, 0, 1};
    Formatter format;

    this->writeBooleanRecord(NVS_UART2_EN, true);
    ESP_LOGI(tag.c_str(), "%s", "WRITING INFO");

    this->writeDwordRecord(NVS_DEVICE_ID, 1234091869);
    this->writeByteRecord(NVS_WIFI_MODE, WifiMode::ApStation);

    this->writeStringRecord(NVS_AP_SSID, "BERDUGO_ESP");
    this->writeStringRecord(NVS_AP_PASSWORD, "123456789");
    this->writeByteRecord(NVS_AP_AUTH_MODE, wifi_auth_mode_t::WIFI_AUTH_WPA2_PSK);
    this->writeStringRecord(NVS_AP_MAC, format.macToString(mac, sizeof(mac)));
    this->writeStringRecord(NVS_AP_IP_ADDRESS, format.ipAddressToString(ipAddress));
    this->writeStringRecord(NVS_AP_SUBNET, format.ipAddressToString(subnet));
    this->writeStringRecord(NVS_AP_GATEWAY, format.ipAddressToString(gateway));
    this->writeBooleanRecord(NVS_AP_DHCP_ENABLE, true);

    this->writeStringRecord(NVS_STA_SSID, "");
    this->writeStringRecord(NVS_STA_PASSWORD, "");
    this->writeByteRecord(NVS_STA_AUTH_MODE, wifi_auth_mode_t::WIFI_AUTH_OPEN);
    this->writeStringRecord(NVS_STA_TARGET_MAC, "");
    this->writeStringRecord(NVS_STA_MAC, "");
    this->writeStringRecord(NVS_STA_IP_ADDRESS, "");
    this->writeStringRecord(NVS_STA_SUBNET, "");
    this->writeStringRecord(NVS_STA_GATEWAY, "");
    this->writeBooleanRecord(NVS_STA_DHCP_ENABLE, true);
    this->writeStringRecord(NVS_STA_API_HOST, "");

    ESP_LOGI(tag.c_str(), "%s", "FINSIHED");
}

WifiConfig LocalStorage::readWifiConfig()
{
    Formatter format;
    WifiConfig config;
    uint8_t ip[4];
    uint8_t mac[6];
    uint8_t macSize = sizeof(mac);
    uint8_t ipSize = sizeof(ip);

    memset(config.ApConfig.mac, 0, sizeof(config.ApConfig.mac));
    memset(config.ApConfig.ip.ip, 0, sizeof(config.ApConfig.ip.ip));
    memset(config.ApConfig.ip.mask, 0, sizeof(config.ApConfig.ip.mask));
    memset(config.ApConfig.ip.gateway, 0, sizeof(config.ApConfig.ip.gateway));

    memset(config.StaConfig.mac, 0, sizeof(config.StaConfig.mac));
    memset(config.StaConfig.ip.ip, 0, sizeof(config.StaConfig.ip.ip));
    memset(config.StaConfig.ip.mask, 0, sizeof(config.StaConfig.ip.mask));
    memset(config.StaConfig.ip.gateway, 0, sizeof(config.StaConfig.ip.gateway));

    ESP_LOGI(tag.c_str(), "%s", "Reading Wifi Configuration");
    config.mode = static_cast<WifiMode>(this->readByteRecord(NVS_WIFI_MODE));

    ESP_LOGI(tag.c_str(), "%s", "Reading Ap Configuration");
    config.ApConfig.authentication = static_cast<WifiMode>(this->readByteRecord(NVS_AP_AUTH_MODE));
    config.ApConfig.ssid = this->readStringRecord(NVS_AP_SSID);
    config.ApConfig.password = this->readStringRecord(NVS_AP_PASSWORD);
    config.ApConfig.dhcpEnabled = this->readBooleanRecord(NVS_AP_DHCP_ENABLE);

    ESP_LOGI(tag.c_str(), "%s", "Reading AP ipAddress");
    format.stringToMac(mac, this->readStringRecord(NVS_AP_MAC));
    memcpy(config.ApConfig.mac, mac, macSize);
    format.stringToIpAddress(ip, this->readStringRecord(NVS_AP_IP_ADDRESS));
    memcpy(config.ApConfig.ip.ip, ip, ipSize);
    format.stringToIpAddress(ip, this->readStringRecord(NVS_AP_SUBNET));
    memcpy(config.ApConfig.ip.mask, ip, ipSize);
    format.stringToIpAddress(ip, this->readStringRecord(NVS_AP_GATEWAY));
    memcpy(config.ApConfig.ip.gateway, ip, ipSize);

    ESP_LOGI(tag.c_str(), "%s", "Reading Sta Configuration");
    config.StaConfig.authentication = static_cast<WifiMode>(this->readByteRecord(NVS_STA_AUTH_MODE));
    config.StaConfig.ssid = this->readStringRecord(NVS_STA_SSID);
    config.StaConfig.password = this->readStringRecord(NVS_STA_PASSWORD);
    config.StaConfig.dhcpEnabled = this->readBooleanRecord(NVS_STA_DHCP_ENABLE);

    ESP_LOGI(tag.c_str(), "%s", "Reading Sta ipAddress");
    format.stringToMac(mac, this->readStringRecord(NVS_STA_MAC));
    memcpy(config.StaConfig.mac, mac, macSize);
    format.stringToMac(mac, this->readStringRecord(NVS_STA_TARGET_MAC));
    memcpy(config.StaConfig.targetMac, mac, macSize);
    format.stringToIpAddress(ip, this->readStringRecord(NVS_STA_IP_ADDRESS));
    memcpy(config.StaConfig.ip.ip, ip, ipSize);
    format.stringToIpAddress(ip, this->readStringRecord(NVS_STA_SUBNET));
    memcpy(config.StaConfig.ip.mask, ip, ipSize);
    format.stringToIpAddress(ip, this->readStringRecord(NVS_STA_GATEWAY));
    memcpy(config.StaConfig.ip.gateway, ip, ipSize);

    ESP_LOGI(tag.c_str(), "%s", "Finish Wifi Configuration");
    return config;
}

ApiConfig LocalStorage::getApiConfig()
{
    ApiConfig config;
    ESP_LOGI(tag.c_str(), "%s", "Reading Sta Api configuration");
    config.host = this->readStringRecord(NVS_STA_API_HOST);
    return config;
}

void LocalStorage::saveStationTarget(ApRecordList record, string password)
{
    this->writeStringRecord(NVS_STA_TARGET_MAC, Formatter::macToString(record.mac, sizeof(record.mac)));
    this->writeStringRecord(NVS_STA_SSID, string(record.ssid));
    this->writeByteRecord(NVS_STA_AUTH_MODE, record.authMode);
    this->writeStringRecord(NVS_STA_PASSWORD, password);
}

string LocalStorage::readStringRecord(string _key)
{
    const char *key = _key.c_str();
    char record[50];
    size_t _size = sizeof(record);
    memset(record, 0, _size);
    if (this->open(_key, NVS_READONLY))
        this->errorCheck(nvs_get_str(handle, key, record, &_size), key);
    nvs_close(handle);
    return string(record);
}

bool LocalStorage::writeStringRecord(string _key, string record)
{
    bool writed = false;
    const char *key = _key.c_str();
    if (this->open(_key, NVS_READWRITE))
    {
        writed = this->errorCheck(nvs_set_str(handle, key, record.c_str()), _key);
        nvs_commit(handle);
        nvs_close(handle);
        return writed;
    }
    ESP_LOGE(tag.c_str(), "Failed writing  string: '%s'", key);
    return false;
}

bool LocalStorage::readBooleanRecord(string _key)
{
    return this->readByteRecord(_key) == 0 ? false : true;
}

bool LocalStorage::writeBooleanRecord(string _key, bool record)
{
    return this->writeByteRecord(_key, record);
}

uint8_t LocalStorage::readByteRecord(string _key)
{
    uint8_t value;
    const char *key = _key.c_str();
    if (this->open(_key, NVS_READONLY))
        this->errorCheck(nvs_get_u8(handle, key, &value), key);
    nvs_close(handle);
    return value;
}

bool LocalStorage::writeByteRecord(string _key, uint8_t record)
{
    bool writed = false;
    const char *key = _key.c_str();
    if (this->open(_key, NVS_READWRITE))
    {
        writed = this->errorCheck(nvs_set_u8(handle, key, record), _key);
        nvs_commit(handle);
        nvs_close(handle);
        return writed;
    }
    ESP_LOGE(tag.c_str(), "Failed writing  uint32: '%s'", key);
    return false;
}

int64_t LocalStorage::readDwordRecord(string _key)
{
    int64_t value;
    const char *key = _key.c_str();
    if (this->open(_key, NVS_READONLY))
        this->errorCheck(nvs_get_i64(handle, key, &value), key);
    nvs_close(handle);
    return value;
}

bool LocalStorage::writeDwordRecord(string _key, int64_t record)
{
    bool writed = false;
    const char *key = _key.c_str();
    if (this->open(_key, NVS_READWRITE))
    {
        writed = this->errorCheck(nvs_set_i64(handle, key, record), _key);
        nvs_commit(handle);
        nvs_close(handle);
        return writed;
    }
    ESP_LOGE(tag.c_str(), "Failed writing int64: '%s'", key);
    return false;
}

bool LocalStorage::open(string key, nvs_open_mode_t openMode)
{
    if (this->errorCheck(nvs_open(key.c_str(), openMode, &handle), key))
        return true;
    ESP_LOGE(tag.c_str(), "Failed at open: '%s' ", key.c_str());
    return false;
}

bool LocalStorage::errorCheck(esp_err_t err, string _key)
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

void LocalStorage::formatPartition()
{
    ESP_LOGI(tag.c_str(), "%s", "Fomatting local memory");
    nvs_erase_all(handle);
}

LocalStorage::~LocalStorage()
{
}