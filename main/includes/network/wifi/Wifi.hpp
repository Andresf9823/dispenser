#ifndef _WIFI_SERVICE_HPP_
#define _WIFI_SERVICE_HPP_

#include <GlobalDefines.hpp>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_wifi.h>
#include "../NetworkInterface.hpp"

#define MAXIMUM_SIZE_OF_SCAN_LIST 10

#define DEFAULT_WIFI_MAC_0 0x00
#define DEFAULT_WIFI_MAC_1 0x01
#define DEFAULT_WIFI_MAC_2 0x0A
#define DEFAULT_WIFI_MAC_3 0xA0
#define DEFAULT_WIFI_MAC_4 0x10
#define DEFAULT_WIFI_MAC_5 0x00

typedef enum _WifiMode
{
	Unkown,
	Station,
	Ap,
	ApStation
} WifiMode;

typedef struct _ApRecordList
{
	char ssid[33];
	uint8_t mac[6];
	uint8_t authMode;
	uint8_t rssi;
} ApRecordList;

typedef struct _WifiConfig
{
	WifiMode mode;
	NetworkProperties ApConfig;
	NetworkProperties StaConfig;
} WifiConfig;

class WifiService : public Tcp
{
private:
	static constexpr string tag = "WIFI SERVICE";
	ApRecordList apRecordsScanned[MAXIMUM_SIZE_OF_SCAN_LIST];
	uint8_t ApAuthenticationMode;
	string ApPassword;
	string StaPassword;
	wifi_config_t wifi_config;
	void setApConfig(WifiConfig config);
	void setStationConfig(WifiConfig config);
	void setIpAddress(WifiMode mode, NetworkProperties ipConfig);
	bool macSafeValidator(wifi_interface_t interface, uint8_t *mac);
	NetworkProperties getApConfig();
	NetworkProperties getStaConfig();
	static wifi_config_t defaultWifiConfig;
	static void wifiEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

public:
	WifiService();
	bool init(WifiConfig config);
	uint16_t scanWifiNetworks(ApRecordList *apRecords);
	ApRecordList getRecordScannned(uint8_t index);
	WifiConfig getConfig();
	~WifiService();
};
#endif