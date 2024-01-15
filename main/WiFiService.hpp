#ifndef _WIFI_SERVICE_HPP_
#define _WIFI_SERVICE_HPP_

#include <TcpService.hpp>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_wifi.h>
#include <ArduinoJson-v6.21.3.h>

#define MAXIMUM_SIZE_OF_SCAN_LIST 10

typedef enum WiFiMode
{
	ApStation,
	Station,
	Ap
} _WifiMode;

typedef struct _ApRecordList
{
	char ssid[33];
	char mac[18];
	uint8_t authMode;
	uint8_t rssi;
} ApRecordList;

class WifiService : public TcpService
{
private:
	static constexpr char *tag = "WIFI SERVICE";
	wifi_config_t wifi_config;
	void SetApConfig();
	void SetStationConfig();
	static wifi_config_t defaultWifiConfig;
	static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

public:
	WifiService();
	bool InitWifiService(WiFiMode mode);
	uint16_t ScanWifiNetworks(ApRecordList *apRecords);
	~WifiService();
};
#endif
