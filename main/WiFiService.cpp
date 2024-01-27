#include "WiFiService.hpp"

WifiService::WifiService(/* args */) // @suppress("Class members should be properly initialized")
{
}

void WifiService::WifiEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT)
    {

        if (event_id == WIFI_EVENT_AP_START)
        {
            wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
            if (event != NULL)
            {
            }
        }
        else if (event_id == WIFI_EVENT_AP_STACONNECTED)
        {
            wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
            if (event != NULL)
            {
                ESP_LOGI("AP CONNECTED", "ID: %d MAC:%02X:%02X:%02X:%02X:%02X:%02X",
                         event->aid, event->mac[0], event->mac[1], event->mac[2],
                         event->mac[3], event->mac[4], event->mac[5]);
                if (event->is_mesh_child)
                {
                    ESP_LOGI("AP CONNECTED", "IS A MESH CHILD");
                }
            }
        }
        else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
        {
            wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
            if (event != NULL)
            {
                ESP_LOGI("AP DISCONECTED",
                         "ID: %d MAC:%02X:%02X:%02X:%02X:%02X%02X", event->aid,
                         event->mac[0], event->mac[1], event->mac[2], event->mac[3],
                         event->mac[4], event->mac[5]);
                if (event->is_mesh_child)
                {
                    ESP_LOGI("AP DISCONECTED", "WAS A MESH CHILD");
                }
            }
        }
        else if (WIFI_EVENT_SCAN_DONE)
        {
            wifi_event_sta_scan_done_t *event = (wifi_event_sta_scan_done_t *)event_data;
            if (event != NULL)
            {
                if (event->status == 0)
                {
                    ESP_LOGI("SCAN DONE", "Results %d", event->number);
                    ESP_LOGI("SCAN DONE", "Scan ID's %d", event->scan_id);
                }
                else
                {
                    ESP_LOGI("SCAN FAILED", "The WiFi scanning failed for some reason");
                }
            }
        }
    }
    else if (event_base == IP_EVENT)
    {

        if (event_id == IP_EVENT_STA_GOT_IP || event_id == IP_EVENT_ETH_GOT_IP || event_id == IP_EVENT_PPP_GOT_IP)
        {
            ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
            if (event != NULL)
            {

                ESP_LOGI("DEVICE GOT IP", "IP: "
                                          "%d.%d.%d.%d",
                         IP2STR(&event->ip_info.ip));
                ESP_LOGI("DEVICE GOT IP", "MASK: "
                                          "%d.%d.%d.%d",
                         IP2STR(&event->ip_info.netmask));
                ESP_LOGI("DEVICE GOT IP", "GATEWAY: "
                                          "%d.%d.%d.%d",
                         IP2STR(&event->ip_info.gw));
            }
        }
        else if (event_id == IP_EVENT_AP_STAIPASSIGNED)
        {
            ip_event_ap_staipassigned_t *event = (ip_event_ap_staipassigned_t *)event_data;
            if (event != NULL)
            {
                char mac[18];
                sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", event->mac[0],
                        event->mac[1], event->mac[2], event->mac[3], event->mac[4],
                        event->mac[5]);
                ESP_LOGI("AP_STA IP ASSIGNED:", "MAC: %s", (const char *)mac);
                ESP_LOGI("AP_STA IP ASSIGNED:", "IP: %s",
                         (const char *)event->ip.addr);
            }
        }
        else if (event_id == IP_EVENT_STA_LOST_IP || event_id == IP_EVENT_ETH_LOST_IP || event_id == IP_EVENT_PPP_LOST_IP)
        {
            ESP_LOGI("DEVICE LOST IP", "IP, MASK AND GW LOST");
        }
    }
}

void WifiService::SetStationConfig(WifiConfig config)
{
    this->logString(tag, "Setting Station configuration");
    this->SetIpAddress(NetworkInterface::WifiStation, config.StaConfig);

    wifi_config = {.sta =
                       {
                           .ssid = {'M', 'A', 'R', 'Y'},
                           .password = {'3', '2', '7', '2', '7', '7', '4', '7'},
                           .scan_method = WIFI_ALL_CHANNEL_SCAN,
                           .bssid_set = 0,
                           .bssid = {0x00, 0x01, 0x0A, 0x10, 0x00, 0x02}, /**< MAC address of target AP*/
                                                                          //    .channel = 0,
                                                                          //    .listen_interval = 3,
                                                                          //    .sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
                           .threshold = (wifi_scan_threshold_t){.rssi = 99, .authmode = WIFI_AUTH_WPA2_PSK},
                           //    .pmf_cfg = (wifi_pmf_config_t){.capable = true, .required = false},
                           //    .rm_enabled = (uint32_t)1,
                           //    .btm_enabled = (uint32_t)1,
                           //    .mbo_enabled = (uint32_t)1,
                           //    //		.bo_enabled = (uint32_t)1,
                           //    .ft_enabled = (uint32_t)1,
                           //    .owe_enabled = (uint32_t)1,
                           //    .transition_disable = (uint32_t)1,
                           //    .reserved = (uint32_t)26,
                           //    .sae_pwe_h2e = WPA3_SAE_PWE_UNSPECIFIED,
                           //    .failure_retry_cnt = 5,
                       }};
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
}

NetworkIpAddress WifiService::GetStaConfig()
{
    logString(tag, "Searching Station Config");
    NetworkIpAddress station;
    wifi_ap_record_t ap;

    if (ESP_OK != esp_wifi_sta_get_ap_info(&ap))
    {
        station.ssid = "NULL";
        station.auth = 0;
        station.mode = WIFI_MODE_STA;
        station.password = "NULL";
        memset(station.mac, 0, sizeof(station.mac));
        memset(station.ip, 0, sizeof(station.ip));
        memset(station.mask, 0, sizeof(station.mask));
        memset(station.gateway, 0, sizeof(station.gateway));
    }
    else
    {
        esp_netif_ip_info_t ipInfo;
        esp_netif_get_ip_info(esp_netif_sta, &ipInfo);

        station.ssid = (const char *)&ap.ssid;
        station.auth = ap.authmode;
        station.mode = WIFI_MODE_STA;
        station.password = "NULL";
        memcpy(station.mac, ap.bssid, sizeof(ap.bssid));
        memcpy(station.ip, &ipInfo.ip, sizeof(ipInfo.ip));
        memcpy(station.mask, &ipInfo.netmask, sizeof(ipInfo.netmask));
        memcpy(station.gateway, &ipInfo.gw, sizeof(ipInfo.gw));
    }
    logString(tag, "Finish");
    return station;
}

void WifiService::SetApConfig(WifiConfig config)
{
    this->logString(tag, "Setting Access Point configuration");
    this->SetIpAddress(NetworkInterface::WifiAp, config.ApConfig);

    wifi_config = {.ap =
                       {
                           .ssid = {'B', 'E', 'R', 'D', 'U', 'G', 'O', '_', 'E', 'S', 'P'},
                           .password = {'1', '2', '3', '4', '5', '6', '7', '8', '9'},
                           .ssid_len = sizeof("BERDUGO_ESP") - 1,
                           //    .channel = (uint8_t)10,
                           .authmode = WIFI_AUTH_WPA2_PSK,
                           .ssid_hidden = 0,
                           .max_connection = (uint8_t)5,
                           .beacon_interval = 100,
                           .pairwise_cipher = WIFI_CIPHER_TYPE_NONE,
                           //    .ftm_responder = 1,
                           .pmf_cfg = (wifi_pmf_config_t){.capable = true, .required = false}}};
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
}

NetworkIpAddress WifiService::GetApConfig()
{
    logString(tag, "Searching Ap Config");
    NetworkIpAddress apConfig;
    esp_netif_ip_info_t ipInfo;
    const char *hostName;
    uint8_t mac[6];
    esp_netif_get_ip_info(esp_netif_ap, &ipInfo);
    esp_netif_get_hostname(esp_netif_ap, &hostName);
    esp_netif_get_mac(esp_netif_ap, mac);

    apConfig.ssid = string(hostName);
    apConfig.auth = WIFI_AUTH_WPA2_PSK;
    apConfig.mode = WIFI_MODE_AP;
    apConfig.password = esp_netif_get_ifkey(esp_netif_ap);

    memcpy(apConfig.mac, mac, sizeof(mac));
    memcpy(apConfig.ip, &ipInfo.ip, sizeof(ipInfo.ip));
    memcpy(apConfig.mask, &ipInfo.netmask, sizeof(ipInfo.netmask));
    memcpy(apConfig.gateway, &ipInfo.gw, sizeof(ipInfo.gw));

    logString(tag, "Finish");
    return apConfig;
}

bool WifiService::InitWifiService(WifiConfig config)
{
    esp_netif_init();
    nvs_flash_init();
    esp_event_loop_create_default();

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &WifiEventHandler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &WifiEventHandler, NULL, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    logString(tag, "-> Initializing WiFi mode <-");

    switch (config.mode)
    {
    case WiFiMode::Ap:
        esp_wifi_set_mode(WIFI_MODE_AP);
        SetApConfig(config);
        break;
    case WiFiMode::Station:
        esp_wifi_set_mode(WIFI_MODE_STA);
        SetStationConfig(config);
        break;
    case WiFiMode::ApStation:
        esp_wifi_set_mode(WIFI_MODE_APSTA);
        SetApConfig(config);
        SetStationConfig(config);
        break;
    default:
        logString(tag, "Wifi mode unknown");
        return false;
        break;
    }
    if (esp_wifi_start() == ESP_OK)
    {
        CreateSocket(1100);
        return true;
    }
    return false;
}

uint16_t WifiService::ScanWifiNetworks(ApRecordList *apRecords)
{
    logString(tag, "Start WiFi networks scanning");
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

WifiConfig WifiService::GetConfig()
{
    WifiConfig config;
    wifi_mode_t mode;
    esp_wifi_get_mode(&mode);
    config.mode = (WiFiMode)mode;

    switch (config.mode)
    {
    case WiFiMode::Ap:
        config.ApConfig = this->GetApConfig();
        break;
    case WiFiMode::Station:
        config.StaConfig = this->GetStaConfig();
        break;
    case WiFiMode::ApStation:
        config.ApConfig = this->GetApConfig();
        config.StaConfig = this->GetStaConfig();
        break;
    default:
        config.mode = WiFiMode::Unkown;
        break;
    }
    return config;
}
