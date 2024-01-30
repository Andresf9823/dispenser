#ifndef _WIFI_SERVICE_HPP_
#define _WIFI_SERVICE_HPP_

#include <GlobalDefines.hpp>
#include <TcpService.hpp>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_wifi.h>
#include <ArduinoJson-v6.21.3.h>

#define MAXIMUM_SIZE_OF_SCAN_LIST 10

#define DEFAULT_WIFI_MAC_0 0x00
#define DEFAULT_WIFI_MAC_1 0x01
#define DEFAULT_WIFI_MAC_2 0x0A
#define DEFAULT_WIFI_MAC_3 0xA0
#define DEFAULT_WIFI_MAC_4 0x10
#define DEFAULT_WIFI_MAC_5 0x00

typedef enum _WiFiMode
{
	Unkown,
	Station,
	Ap,
	ApStation
} WiFiMode;

typedef struct _ApRecordList
{
	char ssid[33];
	uint8_t mac[6];
	uint8_t authMode;
	uint8_t rssi;
} ApRecordList;

typedef struct _WifiConfig
{
	WiFiMode mode;
	NetworkIpAddress ApConfig;
	NetworkIpAddress StaConfig;
} WifiConfig;

class WifiService : public TcpService
{
private:
	static constexpr string tag = "WIFI SERVICE";
	uint8_t ApAuthenticationMode;
	string ApPassword;
	string StaPassword;
	wifi_config_t wifi_config;
	void SetApConfig(WifiConfig config);
	void SetStationConfig(WifiConfig config);
	bool MacSafeValidator(wifi_interface_t interface, uint8_t *mac);
	NetworkIpAddress GetApConfig();
	NetworkIpAddress GetStaConfig();
	static wifi_config_t defaultWifiConfig;
	static void WifiEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

public:
	WifiService();
	bool InitWifiService(WifiConfig config);
	uint16_t ScanWifiNetworks(ApRecordList *apRecords);
	WifiConfig GetConfig();
	~WifiService();
};
#endif
