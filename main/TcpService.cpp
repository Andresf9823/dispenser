#include "TcpService.hpp"

TcpService::TcpService(/* args */)
{
}

void TcpService::InitTcpService()
{
    nvs_flash_init();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
}

void TcpService::ScanWifiNetworks()
{

    uint16_t numberOfApScanned = 0;
    wifi_ap_record_t apRecords[maximumSizeOfScanList];
    memset(apRecords, 0, maximumSizeOfScanList);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    esp_wifi_scan_start(NULL, true);
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&maximumSizeOfScanList, apRecords));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&numberOfApScanned));

    for (uint8_t i = 0; i < numberOfApScanned; i++)
    {
        this->logString(this->tag, (const char *)apRecords[i].ssid);
    }
}

TcpService::~TcpService()
{
}