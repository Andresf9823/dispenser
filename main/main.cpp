#include <GlobalDefines.hpp>
#include "domain/network/wifi/Wifi.hpp"
#include "domain/database/localstorage/LocalStorage.hpp"

static string tag = "MAIN";

Wifi *wifi;
LocalStorage *localStorage;

void logString(string TAG, string message)
{
	ESP_LOGI((const char *)TAG.c_str(), "%s", (unsigned char *)message.c_str());
}

void logDword(string TAG, int64_t logNumber)
{
	logString(TAG, to_string(logNumber));
}

void logFloat(string TAG, double logFloating)
{
	logString(TAG, to_string(logFloating));
}
/*
void SendWifiApRecordsScanned()
{
	ApRecordList apRecords[MAXIMUM_SIZE_OF_SCAN_LIST];
	uint16_t recordsScanned = Wifi->ScanWifiNetworks(apRecords);
	Wifi->SendTcpMessage(Format->apRecordsList(apRecords, recordsScanned));
}

void SendDeviceInfo()
{
	DeviceInformation deviceInfo;

	deviceInfo.deviceId = File->ReadUint32tRecord(NVS_DEVICE_ID);
	deviceInfo.versionApp = VERSION_APP;
	deviceInfo.wifiConfig = Wifi->GetConfig();
	deviceInfo.WifiApiClient = Wifi->ApiSta->GetConfig();
	Wifi->SendTcpMessage(Format->deviceInformation(deviceInfo));
}

void SetDefaultMemoryValues()
{
	CommandResult result;
	result.command = ProtocolCommand::setDefaultMemoryValues;
	result.deviceId = File->ReadUint32tRecord(NVS_DEVICE_ID);
	result.status = true;
	result.message = "Default values loaded, please restart system";
	File->SetDefaultValues();
	Wifi->SendTcpMessage(Format->reportComandResult(result));
}

void SaveWifiApRecord()
{
}

void RestartSystem()
{
	CommandResult result;
	result.command = ProtocolCommand::restartSystem;
	result.deviceId = File->ReadUint32tRecord(NVS_DEVICE_ID);
	result.status = true;
	result.message = "RESTARTING SYSTEM IN 3 SECONDS";
	logString(tag, result.message);
	Wifi->SendTcpMessage(Format->reportComandResult(result));
	for (uint8_t i = 0; i < 3; i++)
	{
		logString(tag, ".");
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
	Wifi->SendTcpMessage("Restarting");
	esp_restart();
}

*/
void initObjects()
{
	localStorage = new LocalStorage();
	localStorage->setDefaultValues();

	wifi = new Wifi();
	wifi->logString = logString;
	wifi->logDword = logDword;
	wifi->logFloat = logFloat;

	wifi->init(new IWifiModeSta());
}

extern "C" void app_main(void)
{
	logString(tag, "Go project!");
	initObjects();
	while (true)
	{
		vTaskDelay(1);
	}
}
