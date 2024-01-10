#include <GlobalDefines.hpp>
#include <esp_log.h>
#include <Formatter.hpp>
#include <InputsOutputs.hpp>
#include <WiFiService.hpp>
#include <UartsFunctions.hpp>

using namespace std;

static const char *tag = "Main";

Uarts *Uart;
Formatter *Format;
InputsOutputs *Gpio;
WifiService *Wifi;

void logString(const char *TAG, const char *message)
{
	ESP_LOGI(TAG, "%s", (unsigned char *)message);
}

void logDword(const char *TAG, uint32_t logNumber)
{
	char charNumber[KB];
	sprintf(charNumber, "%ld", logNumber);
	logString(TAG, charNumber);
}

void logFloat(const char *TAG, double logFloating)
{
	char charFloating[KB];
	sprintf(charFloating, "%f.2", logFloating);
	logString(TAG, (char *)charFloating);
}

static void SendWifiApRecordsScanned()
{
	ApRecordList apRecords[MAXIMUM_SIZE_OF_SCAN_LIST];
	uint16_t recordsScanned = Wifi->ScanWifiNetworks(apRecords);

	// 	logString(tag, apRecords[i].mac);
	// 	logString(tag, apRecords[i].ssid);
	// 	logDword(tag, apRecords[i].authMode);
	// 	logDword(tag, apRecords[i].rssi);

	Format->apRecordsListToJson(apRecords, recordsScanned);
}

void initObjects()
{
	Uart = new Uarts();
	Uart->logString = logString;
	Uart->logDword = logDword;
	Uart->logFloat = logFloat;
	Uart->UartInitializer(2);

	Format = new Formatter();
	Format->logString = logString;
	Format->logDword = logDword;
	Format->logFloat = logFloat;

	Gpio = new InputsOutputs();
	Gpio->logString = logString;
	Gpio->logDword = logDword;
	Gpio->logFloat = logFloat;
	Gpio->InitBlink();

	Wifi = new WifiService();
	Wifi->logString = logString;
	Wifi->logDword = logDword;
	Wifi->logFloat = logFloat;
	Wifi->InitWifiService(WiFiMode::ApStation);
	Wifi->SendWifiApRecordsScanned = <static_cast> SendWifiApRecordsScanned;
}

extern "C" void app_main(void)
{
	logString(tag, "Go project!");
	initObjects();

	while (true)
	{
		// Wifi->TcpAppStack();
	}
}
