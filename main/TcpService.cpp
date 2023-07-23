#include "TcpService.hpp"

TcpService::TcpService(/* args */)
{
    maximumSizeOfScanList = 10;
}

void TcpService::wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT)
    {
        if (event_id == WIFI_EVENT_AP_START)
        {
            wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
            ScanWifiNetworks();
        }
        else if (event_id == WIFI_EVENT_AP_STACONNECTED)
        {
            wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
        }
        else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
        {
            wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
        }
    }
}

void TcpService::InitTcpService(WiFiMode mode)
{
    nvs_flash_init();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    switch (mode)
    {
    case WiFiMode::ApStation:
        /* code */
        break;
    case WiFiMode::Station:
        /* code */
        break;
    case WiFiMode::Ap:
        wifi_config_t wifi_config;
        wifi_config.ap = (wifi_ap_config_t){
            .ssid = {'B', 'E', 'R', 'D', 'U', 'G', 'O', '_', 'E', 'S', 'P'},
            .password = {'1', '2', '3', '4', '5', '6', '7', '8', '9'},
            .ssid_len = sizeof("BERDUGO_ESP") - 1,
            .channel = (uint8_t)10,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .ssid_hidden = 0,
            .max_connection = (uint8_t)5,
            .beacon_interval = 100,
            .pairwise_cipher = WIFI_CIPHER_TYPE_NONE,
        };

        // if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0)
        // {
        //     wifi_config.ap.authmode = WIFI_AUTH_OPEN;
        // }

        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
        ESP_ERROR_CHECK(esp_wifi_start());

        break;

    default:
        break;
    }
}

void TcpService::ScanWifiNetworks()
{

    uint16_t numberOfApScanned = 0;
    uint16_t maximumSizeOfScanList = MAXIMUM_SIZE_OF_SCAN_LIST;
    wifi_ap_record_t apRecords[maximumSizeOfScanList];
    memset(apRecords, 0, maximumSizeOfScanList);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    esp_wifi_scan_start(NULL, true);
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&maximumSizeOfScanList, apRecords));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&numberOfApScanned));

    for (uint8_t i = 0; i < numberOfApScanned; i++)
    {
        ESP_LOGI("WIFI SCAN:", "%s", (unsigned char *)apRecords[i].ssid);
        ESP_LOGI("WIFI SCAN:", "%d", apRecords[i].rssi);
        ESP_LOGI("WIFI SCAN:", "%d", apRecords[i].authmode);
    }
}

TcpService::~TcpService()
{
}