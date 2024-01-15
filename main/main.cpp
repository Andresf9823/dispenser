#include <GlobalDefines.hpp>
#include <Formatter.hpp>
#include <InputsOutputs.hpp>
#include <WiFiService.hpp>
#include <UartsFunctions.hpp>

using namespace std;

static string tag = "MAIN";

Uarts *Uart;
Formatter *Format;
InputsOutputs *Gpio;
WifiService *Wifi;

void logString(string TAG, string message)
{
	ESP_LOGI((const char *)TAG.c_str(), "%s", (unsigned char *)message.c_str());
}

void logDword(string TAG, int32_t logNumber)
{
	logString(TAG, to_string(logNumber));
}

void logFloat(string TAG, double logFloating)
{
	logString(TAG, to_string(logFloating));
}

void SendWifiApRecordsScanned()
{
	ApRecordList apRecords[MAXIMUM_SIZE_OF_SCAN_LIST];
	uint16_t recordsScanned = Wifi->ScanWifiNetworks(apRecords);
	/*
		logString(tag, apRecords[i].mac);
		logString(tag, apRecords[i].ssid);
		logDword(tag, apRecords[i].authMode);
		logDword(tag, apRecords[i].rssi);
	*/
	Wifi->SendTcpMessage(Format->apRecordsList(apRecords, recordsScanned));
}

void SendDeviceInfo()
{
	/*
		Send a Json by TCP with this information:
		MAC
		Device IP
		ID
		URL's service
		Service IP
		Service PORT
		Station Wifi where is connected, if it does
		Ap mode SSID name
		AP mode SSID password
		Ap mode security
	*/
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
	Wifi->SendWifiApRecordsScanned = SendWifiApRecordsScanned;
	Wifi->SendDeviceInfo = SendDeviceInfo;
	Wifi->InitWifiService(WiFiMode::ApStation);
}

extern "C" void app_main(void)
{
	logString(tag, "Go project!");
	initObjects();

	while (true)
	{
		Wifi->TcpAppStack();
		vTaskDelay(1);
	}
}
