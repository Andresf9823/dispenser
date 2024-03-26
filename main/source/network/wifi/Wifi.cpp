#include "domain/network/wifi/Wifi.hpp"

Wifi::Wifi()
{
}

void Wifi::WifiEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
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

void Wifi::init(IWifiMode *mode)
{
    esp_netif_init();
    nvs_flash_init();
    esp_event_loop_create_default();

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &WifiEventHandler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &WifiEventHandler, NULL, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    logString(tag, "-> Initializing WiFi mode <-");
    LocalStorage storage = LocalStorage();
    storage.readWifiConfiguration(&configuration);
    mode->setConfiguration(configuration);

    if (esp_wifi_start() != ESP_OK)
        logString(tag, "----> Error initializing WiFi mode <----");
}

Wifi::~Wifi()
{
}