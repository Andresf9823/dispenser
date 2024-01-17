#ifndef _WIFI_SERVICE_HPP_
#define _WIFI_SERVICE_HPP_

#include <GlobalDefines.hpp>
#include <TcpService.hpp>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_wifi.h>
#include <ArduinoJson-v6.21.3.h>

#define MAXIMUM_SIZE_OF_SCAN_LIST 10

#define IP_ADDRESS_0 192
#define IP_ADDRESS_1 168
#define IP_ADDRESS_2 0
#define IP_ADDRESS_3 1

#define NETMASK_0 192
#define NETMASK_1 168
#define NETMASK_2 0
#define NETMASK_3 1

#define GATEWAY_0 192
#define GATEWAY_1 168
#define GATEWAY_2 0
#define GATEWAY_3 1

#define MAC_0 0x00
#define MAC_1 0x0A
#define MAC_2 0x00
#define MAC_3 0x01
#define MAC_4 0x00
#define MAC_5 0xFF

typedef enum WiFiMode
{
	Unkown,
	Station,
	Ap,
	ApStation
} _WifiMode;

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

static esp_netif_t *esp_netif_ap;
static esp_netif_t *esp_netif_sta;

class WifiService : public TcpService
{
private:
	static constexpr string tag = "WIFI SERVICE";
	wifi_config_t wifi_config;
	void SetApConfig(bool dhcpEnlabled);
	void SetStationConfig(bool dhcpEnlabled);
	NetworkIpAddress GetApConfig();
	NetworkIpAddress GetStaConfig();
	static wifi_config_t defaultWifiConfig;
	static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

public:
	WifiService();
	bool InitWifiService(WiFiMode mode, bool apDhcp, bool staDhcp);
	uint16_t ScanWifiNetworks(ApRecordList *apRecords);
	WifiConfig GetConfig();
	~WifiService();
};
#endif
