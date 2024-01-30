#include <GlobalDefines.hpp>
#include <InputsOutputs.hpp>
#include <UartsFunctions.hpp>
#include <WiFiService.hpp>
#include <FileSystem.hpp>
#include <Formatter.hpp>

using namespace std;

static string tag = "MAIN";

Uarts *Uart;
Formatter *Format;
InputsOutputs *Gpio;
WifiService *Wifi;
FileSystem *File;

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

	deviceInfo.deviceId = File->ReadUint32tRecord(NVS_DEVICE_ID);
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

void initObjects()
{
	File = new FileSystem();

	Uart = new Uarts();
	Uart->logString = logString;
	Uart->logDword = logDword;
	Uart->logFloat = logFloat;
	if (File->ReadBooleanRecord(NVS_UART2_EN))
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
	Wifi->RestartSystem = RestartSystem;
	Wifi->SendDeviceInfo = SendDeviceInfo;
	Wifi->SendWifiApRecordsScanned = SendWifiApRecordsScanned;
	Wifi->SetDefaultMemoryValues = SetDefaultMemoryValues;

	Wifi->InitWifiService(File->ReadWifiConfig());
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
