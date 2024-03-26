#include "domain/network/wifi/modes/ModeSta.hpp"

IWifiModeSta::IWifiModeSta()
{
}

void IWifiModeSta::getConfiguration()
{
}

void IWifiModeSta::setIpAddress(WifiConfiguration config)
{
    ESP_LOGI(tag.c_str(), "Setting Sta Ip Configuration");
    esp_netif_ip_info_t ipInfo;

    IP4_ADDR(&ipInfo.ip, config.ipv4.ip[0], config.ipv4.ip[1], config.ipv4.ip[2], config.ipv4.ip[3]);
    IP4_ADDR(&ipInfo.netmask, config.ipv4.mask[0], config.ipv4.mask[1], config.ipv4.mask[2], config.ipv4.mask[3]);
    IP4_ADDR(&ipInfo.gw, config.ipv4.gateway[0], config.ipv4.gateway[1], config.ipv4.gateway[2], config.ipv4.gateway[3]);

    ESP_LOGW(tag.c_str(), "MAC %c", (char)config.mac[0]);
    ESP_LOGW(tag.c_str(), "MAC %c", (char)config.mac[1]);
    ESP_LOGW(tag.c_str(), "MAC %c", (char)config.mac[2]);
    ESP_LOGW(tag.c_str(), "MAC %c", (char)config.mac[3]);
    ESP_LOGW(tag.c_str(), "MAC %c", (char)config.mac[4]);
    ESP_LOGW(tag.c_str(), "MAC %c", (char)config.mac[6]);

    esp_netif_inherent_config_t interfaceConfig = {.flags = (esp_netif_flags_t)(ESP_NETIF_IPV4_ONLY_FLAGS(ESP_NETIF_DHCP_CLIENT) | ESP_NETIF_DEFAULT_ARP_FLAGS | ESP_NETIF_DEFAULT_MLDV6_REPORT_FLAGS | ESP_NETIF_FLAG_EVENT_IP_MODIFIED),
                                                   .mac = {config.mac[0], config.mac[1], config.mac[2], config.mac[3], config.mac[4], config.mac[5]},
                                                   .ip_info = &ipInfo,
                                                   .get_ip_event = IP_EVENT_STA_GOT_IP,
                                                   .lost_ip_event = IP_EVENT_STA_LOST_IP,
                                                   .if_key = "Station",
                                                   .if_desc = "sta",
                                                   .route_prio = 100,
                                                   .bridge_info = NULL};

    esp_netif_config_t cfg = {.base = &interfaceConfig,
                              .driver = NULL,
                              .stack = ESP_NETIF_NETSTACK_DEFAULT_WIFI_STA};

    esp_netif_t *networkInterface = esp_netif_new(&cfg);
    esp_netif_dhcpc_stop(networkInterface);
    esp_netif_set_hostname(networkInterface, config.ssid.c_str());
    ESP_ERROR_CHECK(esp_netif_set_mac(networkInterface, interfaceConfig.mac));
    esp_netif_set_ip_info(networkInterface, &ipInfo);
    if (config.dhcpEnabled)
        esp_netif_dhcpc_start(networkInterface);
    assert(networkInterface);
    ESP_ERROR_CHECK(esp_netif_attach_wifi_station(networkInterface));
    ESP_ERROR_CHECK(esp_wifi_set_default_wifi_sta_handlers());
}

void IWifiModeSta::setConfiguration(vector<WifiConfiguration> config)
{
    ESP_LOGI(tag.c_str(), "Setting Station configuration");
    esp_wifi_set_mode(WIFI_MODE_STA);
    this->setIpAddress(config[this->index]);

    uint8_t _ssid[32];
    uint8_t _pass[64];
    memset(_ssid, 0, sizeof(_ssid));
    memset(_pass, 0, sizeof(_pass));
    memcpy(_ssid, config[this->index].ssid.c_str(), sizeof(config[this->index].ssid));
    memcpy(_pass, config[this->index].password.c_str(), sizeof(config[this->index].password));
    // this->StaPassword = string((const char *)_pass);
    config[this->index].mac[0] = (char)0x04;
    config[this->index].mac[0] = (char)0x33;
    config[this->index].mac[0] = (char)0xc2;
    config[this->index].mac[0] = (char)0x75;
    config[this->index].mac[0] = (char)0x43;
    config[this->index].mac[0] = (char)0x5b;
    wifi_config_t
        wifi_config = {.sta =
                           {
                               .ssid = {_ssid[0], _ssid[1], _ssid[2], _ssid[3], _ssid[4], _ssid[5], _ssid[6], _ssid[7],
                                        _ssid[8], _ssid[9], _ssid[10], _ssid[11], _ssid[12], _ssid[13], _ssid[14], _ssid[15],
                                        _ssid[16], _ssid[17], _ssid[18], _ssid[19], _ssid[20], _ssid[21], _ssid[22], _ssid[23],
                                        _ssid[24], _ssid[25], _ssid[26], _ssid[27], _ssid[28], _ssid[29], _ssid[30], _ssid[31]},
                               .password = {_pass[0], _pass[1], _pass[2], _pass[3], _pass[4], _pass[5], _pass[6], _pass[7],
                                            _pass[8], _pass[9], _pass[10], _pass[11], _pass[12], _pass[13], _pass[14], _pass[15],
                                            _pass[16], _pass[17], _pass[18], _pass[19], _pass[20], _pass[21], _pass[22], _pass[23],
                                            _pass[24], _pass[25], _pass[26], _pass[27], _pass[28], _pass[29], _pass[30], _pass[31],
                                            _pass[32], _pass[33], _pass[34], _pass[35], _pass[36], _pass[37], _pass[38], _pass[39],
                                            _pass[40], _pass[41], _pass[42], _pass[43], _pass[44], _pass[45], _pass[46], _pass[47],
                                            _pass[48], _pass[49], _pass[50], _pass[51], _pass[52], _pass[53], _pass[54], _pass[55],
                                            _pass[56], _pass[56], _pass[58], _pass[50], _pass[60], _pass[61], _pass[62], _pass[63]},
                               .scan_method = WIFI_ALL_CHANNEL_SCAN,
                               .bssid_set = 0,
                               .bssid = {config[this->index].mac[0], config[this->index].mac[1], config[this->index].mac[2],
                                         config[this->index].mac[3], config[this->index].mac[4], config[this->index].mac[5]}, /**< MAC address of target AP*/
                               //    .channel = 0,
                               //    .listen_interval = 3,
                               //    .sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
                               .threshold = (wifi_scan_threshold_t){.rssi = 99, .authmode = (wifi_auth_mode_t)config[this->index].authentication},
                               //    .pmf_cfg = (wifi_pmf_config_t){.capable = true, .required = false},
                               //    .rm_enabled = (uint32_t)1,
                               //    .btm_enabled = (uint32_t)1,
                               //    .mbo_enabled = (uint32_t)1,client/
                               //    //		.bo_enabled = (uint32_t)1,
                               //    .ft_enabled = (uint32_t)1,
                               //    .owe_enabled = (uint32_t)1,
                               //    .transition_disable = (uint32_t)1,
                               //    .reserved = (uint32_t)26,
                               //    .sae_pwe_h2e = WPA3_SAE_PWE_UNSPECIFIED,
                               //    .failure_retry_cnt = 5,
                           }};
    this->macSafeValidator(0x02, config[this->index].mac);
    esp_wifi_set_mac(WIFI_IF_STA, config[this->index].mac);
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
}

uint16_t IWifiModeSta::scanWifiNetworks(ApRecordList *apRecords)
{
    ESP_LOGI(tag.c_str(), "Start WiFi networks scanning");
    uint16_t numberOfApScanned = 0;
    wifi_ap_record_t apRecordsScanned[MAXIMUM_SIZE_OF_SCAN_LIST];
    uint16_t maximumSizeOfScanList = MAXIMUM_SIZE_OF_SCAN_LIST;
    memset(apRecords, 0, MAXIMUM_SIZE_OF_SCAN_LIST);
    esp_wifi_scan_start(NULL, true);
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&maximumSizeOfScanList, apRecordsScanned));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&numberOfApScanned));
    numberOfApScanned = numberOfApScanned > MAXIMUM_SIZE_OF_SCAN_LIST ? MAXIMUM_SIZE_OF_SCAN_LIST : numberOfApScanned;
    for (uint8_t i = 0; i < numberOfApScanned; i++)
    {
        memcpy(apRecords[i].mac, apRecordsScanned[i].bssid, sizeof(apRecordsScanned[i].bssid));
        memcpy(apRecords[i].ssid, apRecordsScanned[i].ssid, sizeof(apRecordsScanned[i].ssid));
        apRecords[i].rssi = apRecordsScanned[i].rssi;
        apRecords[i].authMode = apRecordsScanned[i].authmode;
    }
    return numberOfApScanned;
}

IWifiModeSta::~IWifiModeSta()
{
}