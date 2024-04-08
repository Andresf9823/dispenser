#include <network/wifi/Wifi.hpp>

WifiService::WifiService()
{
    memset(StaMacTarget, 0, sizeof(StaMacTarget));
}

void WifiService::wifiEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT)
    {

        if (event_id == WIFI_EVENT_AP_START)
        {
            wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
            if (event != NULL)
            {
                esp_wifi_connect();
            }
        }
        else if (event_id == WIFI_EVENT_STA_START)
        {
            ESP_LOGW("WIFI STATION START",
                     "%s",
                     "Wifi Started");
            esp_wifi_connect();
        }
        else if (event_id == WIFI_EVENT_STA_CONNECTED)
        {
            wifi_event_sta_connected_t *event = (wifi_event_sta_connected_t *)event_data;
            if (event != NULL)
            {
                ESP_LOGW("STA CONNECTED", "%s", event->ssid);
                ESP_LOGW("STA CONNECTED", "ID: %d MAC:%02X:%02X:%02X:%02X:%02X:%02X",
                         event->aid, event->bssid[0], event->bssid[1], event->bssid[2],
                         event->bssid[3], event->bssid[4], event->bssid[5]);
                ESP_LOGW("STA CONNECTED", "AUTH MODE: %d", event->authmode);
            }
        }
        else if (event_id == WIFI_EVENT_STA_DISCONNECTED)
        {
            wifi_event_sta_disconnected_t *event = (wifi_event_sta_disconnected_t *)event_data;
            if (event != NULL)
            {
                ESP_LOGW("STA DISCONNECTED", "%s", event->ssid);
                ESP_LOGW("STA DISCONNECTED", "MAC:%02X:%02X:%02X:%02X:%02X:%02X",
                         event->bssid[0], event->bssid[1], event->bssid[2],
                         event->bssid[3], event->bssid[4], event->bssid[5]);
                ESP_LOGW("STA DISCONNECTED", "REASON: %d", event->reason);
                esp_wifi_connect();
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
                ESP_LOGW("AP DISCONECTED",
                         "ID: %d MAC:%02X:%02X:%02X:%02X:%02X:%02X", event->aid,
                         event->mac[0], event->mac[1], event->mac[2], event->mac[3],
                         event->mac[4], event->mac[5]);
                if (event->is_mesh_child)
                {
                    ESP_LOGW("AP DISCONECTED", "WAS A MESH CHILD");
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

void WifiService::setIpAddress(WifiMode mode, NetworkProperties ipConfig)
{

    this->logString(tag, "Setting Ip Configuration");
    esp_netif_ip_info_t ipInfo;
    esp_netif_inherent_config_t interfaceConfig;
    esp_netif_config_t cfg;

    IP4_ADDR(&ipInfo.ip, ipConfig.ip.ip[0], ipConfig.ip.ip[1], ipConfig.ip.ip[2], ipConfig.ip.ip[3]);
    IP4_ADDR(&ipInfo.netmask, ipConfig.ip.mask[0], ipConfig.ip.mask[1], ipConfig.ip.mask[2], ipConfig.ip.mask[3]);
    IP4_ADDR(&ipInfo.gw, ipConfig.ip.gateway[0], ipConfig.ip.gateway[1], ipConfig.ip.gateway[2], ipConfig.ip.gateway[3]);

    switch (mode)
    {
    case WifiMode::Ap:
        this->logString(tag, "WifiAp");
        interfaceConfig = {.flags = (esp_netif_flags_t)(ESP_NETIF_IPV4_ONLY_FLAGS(ESP_NETIF_DHCP_SERVER) | ESP_NETIF_FLAG_AUTOUP),
                           .mac = {ipConfig.mac[0], ipConfig.mac[1], ipConfig.mac[2], ipConfig.mac[3], ipConfig.mac[4], ipConfig.mac[5]},
                           .ip_info = &ipInfo,
                           .get_ip_event = 0,
                           .lost_ip_event = 0,
                           .if_key = "Access Point",
                           .if_desc = "ap",
                           .route_prio = 10,
                           .bridge_info = NULL};

        cfg = {.base = &interfaceConfig,
               .driver = NULL,
               .stack = ESP_NETIF_NETSTACK_DEFAULT_WIFI_AP};

        esp_netif_ap = esp_netif_new(&cfg);
        esp_netif_dhcps_stop(esp_netif_ap);
        esp_netif_set_hostname(esp_netif_ap, ipConfig.ssid.c_str());
        ESP_ERROR_CHECK(esp_netif_set_mac(esp_netif_ap, interfaceConfig.mac));
        esp_netif_set_ip_info(esp_netif_ap, &ipInfo);
        if (ipConfig.dhcpEnabled)
            esp_netif_dhcps_start(esp_netif_ap);
        assert(esp_netif_ap);
        ESP_ERROR_CHECK(esp_netif_attach_wifi_ap(esp_netif_ap));
        ESP_ERROR_CHECK(esp_wifi_set_default_wifi_ap_handlers());
        break;
    case WifiMode::Station:
        this->logString(tag, "WifiStation");
        interfaceConfig = {.flags = (esp_netif_flags_t)(ESP_NETIF_IPV4_ONLY_FLAGS(ESP_NETIF_DHCP_CLIENT) | ESP_NETIF_DEFAULT_ARP_FLAGS | ESP_NETIF_DEFAULT_MLDV6_REPORT_FLAGS | ESP_NETIF_FLAG_EVENT_IP_MODIFIED),
                           .mac = {ipConfig.mac[0], ipConfig.mac[1], ipConfig.mac[2], ipConfig.mac[3], ipConfig.mac[4], ipConfig.mac[5]},
                           .ip_info = &ipInfo,
                           .get_ip_event = IP_EVENT_STA_GOT_IP,
                           .lost_ip_event = IP_EVENT_STA_LOST_IP,
                           .if_key = "Station",
                           .if_desc = "sta",
                           .route_prio = 100,
                           .bridge_info = NULL};

        cfg = {.base = &interfaceConfig,
               .driver = NULL,
               .stack = ESP_NETIF_NETSTACK_DEFAULT_WIFI_STA};

        esp_netif_sta = esp_netif_new(&cfg);
        esp_netif_dhcpc_stop(esp_netif_sta);
        esp_netif_set_hostname(esp_netif_sta, ipConfig.ssid.c_str());
        ESP_ERROR_CHECK(esp_netif_set_mac(esp_netif_sta, interfaceConfig.mac));
        esp_netif_set_ip_info(esp_netif_sta, &ipInfo);
        if (ipConfig.dhcpEnabled)
            esp_netif_dhcpc_start(esp_netif_sta);
        assert(esp_netif_sta);
        ESP_ERROR_CHECK(esp_netif_attach_wifi_station(esp_netif_sta));
        ESP_ERROR_CHECK(esp_wifi_set_default_wifi_sta_handlers());
        break;
    default:
        return;
        break;
    }

    this->logString(tag, "Finish Ip Configuration");
}

void WifiService::setStationConfig(WifiConfig config)
{
    this->logString(tag, "Setting Station configuration");
    this->setIpAddress(WifiMode::Station, config.StaConfig);

    uint8_t _ssid[32];
    uint8_t _pass[64];
    memset(_ssid, 0, sizeof(_ssid));
    memset(_pass, 0, sizeof(_pass));
    memcpy(_ssid, config.StaConfig.ssid.c_str(), sizeof(config.StaConfig.ssid));
    memcpy(_pass, config.StaConfig.password.c_str(), sizeof(config.StaConfig.password));
    memcpy(StaMacTarget, config.StaConfig.targetMac, sizeof(StaMacTarget));
    this->StaPassword = string((const char *)_pass);

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
                           .bssid = {config.StaConfig.targetMac[0], config.StaConfig.targetMac[1], config.StaConfig.targetMac[2],
                                     config.StaConfig.targetMac[3], config.StaConfig.targetMac[4], config.StaConfig.targetMac[5]}, /**< MAC address of target AP*/
                           //    .channel = 0,
                           //    .listen_interval = 3,
                           //    .sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
                           .threshold = (wifi_scan_threshold_t){.rssi = 99, .authmode = (wifi_auth_mode_t)config.StaConfig.authentication},
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
    this->macSafeValidator(WIFI_IF_STA, config.StaConfig.mac);
    esp_wifi_set_mac(WIFI_IF_STA, config.StaConfig.mac);
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
}

NetworkProperties WifiService::getStaConfig()
{
    this->logString(tag, "Searching Station Config");
    NetworkProperties station;
    wifi_ap_record_t ap;

    if (ESP_OK != esp_wifi_sta_get_ap_info(&ap))
    {
        station.ssid = "";
        station.authentication = 0;
        station.password = "";
        memset(station.mac, 0, sizeof(station.mac));
        memset(station.ip.ip, 0, sizeof(station.ip.ip));
        memset(station.ip.mask, 0, sizeof(station.ip.mask));
        memset(station.ip.gateway, 0, sizeof(station.ip.gateway));
        memset(station.targetMac, 0, sizeof(station.targetMac));
    }
    else
    {
        esp_netif_ip_info_t ipInfo;
        esp_netif_get_ip_info(esp_netif_sta, &ipInfo);

        station.ssid = (const char *)&ap.ssid;
        station.authentication = ap.authmode;
        station.password = this->StaPassword;
        memcpy(station.mac, ap.bssid, sizeof(ap.bssid));
        memcpy(station.ip.ip, &ipInfo.ip, sizeof(ipInfo.ip));
        memcpy(station.ip.mask, &ipInfo.netmask, sizeof(ipInfo.netmask));
        memcpy(station.ip.gateway, &ipInfo.gw, sizeof(ipInfo.gw));
    }
    memcpy(station.targetMac, StaMacTarget, sizeof(StaMacTarget));
    this->logString(tag, "Finish");
    return station;
}

void WifiService::setApConfig(WifiConfig config)
{
    this->logString(tag, "Setting Access Point configuration");
    this->setIpAddress(WifiMode::Ap, config.ApConfig);

    uint8_t _ssid[32];
    uint8_t _pass[64];
    memset(_ssid, 0, sizeof(_ssid));
    memset(_pass, 0, sizeof(_pass));
    memcpy(_ssid, config.ApConfig.ssid.c_str(), sizeof(config.ApConfig.ssid));
    memcpy(_pass, config.ApConfig.password.c_str(), sizeof(config.ApConfig.password));
    this->ApPassword = string((const char *)_pass);
    this->ApAuthenticationMode = config.ApConfig.authentication;

    wifi_config = {.ap =
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
                           .ssid_len = (uint8_t)strlen((char *)_ssid),
                           //    .channel = (uint8_t)10,
                           .authmode = (wifi_auth_mode_t)this->ApAuthenticationMode,
                           .ssid_hidden = 0,
                           .max_connection = (uint8_t)10,
                           .beacon_interval = 100,
                           .pairwise_cipher = WIFI_CIPHER_TYPE_NONE,
                           //    .ftm_responder = 1,
                           //    .pmf_cfg = (wifi_pmf_config_t){.capable = true, .required = false},
                       }};
    this->macSafeValidator(WIFI_IF_AP, config.ApConfig.mac);
    esp_wifi_set_mac(WIFI_IF_AP, config.ApConfig.mac);
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
}

NetworkProperties WifiService::getApConfig()
{
    this->logString(tag, "Searching Ap Config");
    NetworkProperties apConfig;
    esp_netif_ip_info_t ipInfo;
    const char *hostName;
    uint8_t mac[6];
    esp_netif_get_ip_info(esp_netif_ap, &ipInfo);
    esp_netif_get_hostname(esp_netif_ap, &hostName);
    esp_netif_get_mac(esp_netif_ap, mac);

    apConfig.ssid = string(hostName);
    apConfig.authentication = this->ApAuthenticationMode;
    apConfig.password = this->ApPassword;

    memcpy(apConfig.mac, mac, sizeof(mac));
    memcpy(apConfig.ip.ip, &ipInfo.ip, sizeof(ipInfo.ip));
    memcpy(apConfig.ip.mask, &ipInfo.netmask, sizeof(ipInfo.netmask));
    memcpy(apConfig.ip.gateway, &ipInfo.gw, sizeof(ipInfo.gw));

    this->logString(tag, "Finish");
    return apConfig;
}

bool WifiService::init(WifiConfig config)
{
    esp_netif_init();
    nvs_flash_init();
    esp_event_loop_create_default();

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifiEventHandler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifiEventHandler, NULL, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    this->logString(tag, "-> Initializing WiFi mode <-");

    switch (config.mode)
    {
    case WifiMode::Ap:
        esp_wifi_set_mode(WIFI_MODE_AP);
        this->setApConfig(config);
        break;
    case WifiMode::Station:
        esp_wifi_set_mode(WIFI_MODE_STA);
        this->setStationConfig(config);
        break;
    case WifiMode::ApStation:
        esp_wifi_set_mode(WIFI_MODE_APSTA);
        this->setApConfig(config);
        this->setStationConfig(config);
        break;
    default:
        this->logString(tag, "Wifi mode unknown");
        return false;
        break;
    }
    if (esp_wifi_start() == ESP_OK)
        return true;
    return false;
}

ApRecordList WifiService::getRecordScannned(uint8_t index)
{
    logString(tag, "Record in last scan");
    logString("SSID", string(apRecordsScanned[index].ssid));
    logDword("RSSID", apRecordsScanned[index].rssi);
    logDword("AUTHMODE", apRecordsScanned[index].authMode);
    return apRecordsScanned[index];
}

uint16_t WifiService::scanWifiNetworks(ApRecordList *apRecords)
{
    this->logString(tag, "Start WiFi networks scanning");
    uint16_t numberOfApScanned = 0;
    uint16_t maximumSizeOfScanList = MAXIMUM_SIZE_OF_SCAN_LIST;
    wifi_ap_record_t _apRecordsScanned[MAXIMUM_SIZE_OF_SCAN_LIST];
    memset(_apRecordsScanned, 0, sizeof(_apRecordsScanned));
    memset(apRecords, 0, MAXIMUM_SIZE_OF_SCAN_LIST);
    esp_wifi_scan_start(NULL, true);
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&maximumSizeOfScanList, _apRecordsScanned));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&numberOfApScanned));
    numberOfApScanned = numberOfApScanned > MAXIMUM_SIZE_OF_SCAN_LIST ? MAXIMUM_SIZE_OF_SCAN_LIST : numberOfApScanned;
    for (uint8_t i = 0; i < numberOfApScanned; i++)
    {
        memcpy(apRecords[i].mac, _apRecordsScanned[i].bssid, sizeof(_apRecordsScanned[i].bssid));
        memcpy(apRecords[i].ssid, _apRecordsScanned[i].ssid, sizeof(_apRecordsScanned[i].ssid));
        apRecords[i].rssi = _apRecordsScanned[i].rssi;
        apRecords[i].authMode = _apRecordsScanned[i].authmode;
    }
    memcpy(apRecordsScanned, apRecords, sizeof(apRecordsScanned));
    return numberOfApScanned;
}

void WifiService::setStaMacTarget(uint8_t *StaMacTarget)
{

    memcpy(this->StaMacTarget, StaMacTarget, sizeof(this->StaMacTarget));
}

bool WifiService::macSafeValidator(wifi_interface_t interface, uint8_t *mac)
{
    if ((mac[0] & (char)0x01) == 0x01)
    {
        mac[0] = DEFAULT_WIFI_MAC_0;
        mac[1] = DEFAULT_WIFI_MAC_1;
        mac[2] = DEFAULT_WIFI_MAC_2;
        mac[3] = DEFAULT_WIFI_MAC_3;
        mac[4] = DEFAULT_WIFI_MAC_4;
        mac[5] = (interface == WIFI_IF_AP ? 0x01 : 0x02);
        this->logString(tag, "Mac[0] byte error, setting defult Mac");
        return false;
    }
    return true;
}

WifiConfig WifiService::getConfig()
{
    WifiConfig config;
    wifi_mode_t mode;
    esp_wifi_get_mode(&mode);
    config.mode = (WifiMode)mode;

    switch (config.mode)
    {
    case WifiMode::Ap:
        config.ApConfig = this->getApConfig();
        break;
    case WifiMode::Station:
        config.StaConfig = this->getStaConfig();
        break;
    case WifiMode::ApStation:
        config.ApConfig = this->getApConfig();
        config.StaConfig = this->getStaConfig();
        break;
    default:
        config.mode = WifiMode::Unkown;
        break;
    }
    return config;
}
