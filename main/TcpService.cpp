#include "TcpService.hpp"

TcpService::TcpService(/* args */) // @suppress("Class members should be properly initialized")
{
}

void TcpService::wifi_event_handler(void *arg, esp_event_base_t event_base,
									int32_t event_id, void *event_data)
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
				ESP_LOGI("AP CONNECTED", "ID: %d MAC:%02X:%02X:%02X:%02X:%02X%02X",
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
					ESP_LOGI("SCAN FAILED",
							 "The WiFi scanning failed for some reason");
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

void TcpService::SetStationConfig()
{
	this->logString(tag, "Setting Station configuration");
	esp_netif_t *sta_netif_sta = esp_netif_create_default_wifi_sta();
	assert(sta_netif_sta);

	wifi_config = {.sta =
					   {
						   .ssid = {'L', 'u', 'a', 'n', 'a'},
						   .password = {'3', '2', '7', '2', '7', '7', '4', '7'},
						   .scan_method = WIFI_ALL_CHANNEL_SCAN,
						   .bssid_set = 0,
						   .bssid = {0x00, 0xA4, 0x03, 0x10, 0x00, 0x01}, /**< MAC address of target AP*/
						   .channel = 0,
						   .listen_interval = 3,
						   .sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
						   .threshold = (wifi_scan_threshold_t){.rssi = 99, .authmode = WIFI_AUTH_WPA2_PSK},
						   .pmf_cfg = (wifi_pmf_config_t){.capable = true, .required = false},
						   .rm_enabled = (uint32_t)1,
						   .btm_enabled = (uint32_t)1,
						   .mbo_enabled = (uint32_t)1,
						   //		.bo_enabled = (uint32_t)1,
						   .ft_enabled = (uint32_t)1,
						   .owe_enabled = (uint32_t)1,
						   .transition_disable = (uint32_t)1,
						   .reserved = (uint32_t)26,
						   .sae_pwe_h2e = WPA3_SAE_PWE_UNSPECIFIED,
						   .failure_retry_cnt = 5,
					   }};
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
}

void TcpService::SetApConfig()
{
	this->logString(tag, "Setting Access Point configuration");
	esp_netif_t *esp_netif_ap = esp_netif_create_default_wifi_ap();
	assert(esp_netif_ap);
	wifi_config = {.ap =
					   {
						   .ssid = {'B', 'E', 'R', 'D', 'U', 'G', 'O', '_', 'E', 'S', 'P'},
						   .password = {'1', '2', '3', '4', '5', '6', '7', '8', '9'},
						   .ssid_len = sizeof("BERDUGO_ESP") - 1,
						   .channel = (uint8_t)10,
						   .authmode = WIFI_AUTH_WPA2_PSK,
						   .ssid_hidden = 0,
						   .max_connection = (uint8_t)5,
						   .beacon_interval = 100,
						   .pairwise_cipher = WIFI_CIPHER_TYPE_NONE,
						   .ftm_responder = 1,
						   .pmf_cfg = (wifi_pmf_config_t){.capable = true, .required = false}}};

	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
}

void TcpService::InitWifiService(WiFiMode mode)
{
	esp_netif_init();
	nvs_flash_init();
	esp_event_loop_create_default();

	ESP_ERROR_CHECK(
		esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));

	ESP_ERROR_CHECK(
		esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL));

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT(); // @suppress("Invalid arguments")
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	this->logString(tag, "Initializing WiFi mode: " + mode);

	switch (mode)
	{
	case WiFiMode::Ap:
		ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
		SetApConfig();
		break;
	case WiFiMode::Station:
		ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
		SetStationConfig();
		break;
	case WiFiMode::ApStation:
		ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
		SetStationConfig();
		SetApConfig();
		break;
	default:
		return;
		break;
	}
	ESP_ERROR_CHECK(esp_wifi_start());

	/*Interfaces WiFi task start now*/
	// if (mode != WiFiMode::Ap)
	// {
	// 	wifi_ap_record_t apRecords[MAXIMUM_SIZE_OF_SCAN_LIST];
	// 	uint16_t recordsScanned = ScanWifiNetworks(apRecords);
	// 	for (uint8_t i = 0; i < recordsScanned; i++)
	// 	{
	// 		this->logString(this->tag, (const char *)apRecords[i].ssid);
	// 		char mac[18];
	// 		sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X",
	// 				apRecords[i].bssid[0], apRecords[i].bssid[1],
	// 				apRecords[i].bssid[2], apRecords[i].bssid[3],
	// 				apRecords[i].bssid[4], apRecords[i].bssid[5]);
	// 		this->logString(tag, (const char *)mac);
	// 		this->logDword(this->tag, apRecords[i].rssi);
	// 		this->logDword(this->tag, apRecords[i].authmode);
	// 	}
	// }
}

uint16_t TcpService::ScanWifiNetworks(ApRecordList *apRecords)
{
	
    this->logString(this->tag, "Start WiFi networks scanning");
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
		sprintf(apRecords[i].mac, "%02X:%02X:%02X:%02X:%02X:%02X",
				apRecordsScanned[i].bssid[0], apRecordsScanned[i].bssid[1],
				apRecordsScanned[i].bssid[2], apRecordsScanned[i].bssid[3],
				apRecordsScanned[i].bssid[4], apRecordsScanned[i].bssid[5]);
		memcpy(apRecords[i].ssid, apRecordsScanned[i].ssid, 33);
		apRecords[i].rssi = apRecordsScanned[i].rssi;
		apRecords[i].authMode = apRecordsScanned[i].authmode;
	}

	return numberOfApScanned;
}
