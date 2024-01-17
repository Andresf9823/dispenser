#include <GlobalDefines.hpp>
#include <InputsOutputs.hpp>
#include <UartsFunctions.hpp>
#include <WiFiService.hpp>
#include <Formatter.hpp>

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
	Wifi->SendTcpMessage(Format->apRecordsList(apRecords, recordsScanned));
}

void SendDeviceInfo()
{
	DeviceInformation deviceInfo;

	deviceInfo.deviceId = (uint32_t)1234980623;
	deviceInfo.versionApp = VERSION_APP;
	deviceInfo.wifiConfig = Wifi->GetConfig();
	/*
		Send a Json by TCP with this information:

		WebServices ->suscribed ->IP
		WebServices ->suscribed ->PORT
		WebServices ->suscribed ->URL's Service
	*/
	Wifi->SendTcpMessage(Format->deviceInformation(deviceInfo));
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
	Wifi->InitWifiService(WiFiMode::ApStation, false, false);
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
