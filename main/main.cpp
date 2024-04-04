#include <GlobalDefines.hpp>
#include <network/wifi/Wifi.hpp>
#include <fileSystem/LocalStorage.hpp>

using namespace std;

TcpServerConfiguration appServerconfig;

static string tag = "MAIN";

Uarts *Uart;
InputsOutputs *Gpio;
WifiService *Wifi;
LocalStorage *Storage;

void logString(string TAG, string message)
{
	ESP_LOGW((const char *)TAG.c_str(), "%s", (unsigned char *)message.c_str());
}

void logDword(string TAG, int64_t logNumber)
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
	uint16_t recordsScanned = Wifi->scanWifiNetworks(apRecords);
	Wifi->sendTcpMessage(Formatter::apRecordsList(apRecords, recordsScanned));
}

void SendDeviceInfo()
{
	DeviceInformation deviceInfo;

	deviceInfo.deviceId = Storage->readUint32tRecord(NVS_DEVICE_ID);
	deviceInfo.versionApp = VERSION_APP;
	deviceInfo.wifiConfig = Wifi->getConfig();
	// deviceInfo.WifiApiClient = Wifi->ApiSta->GetConfig();
	Wifi->sendTcpMessage(Formatter::deviceInformation(deviceInfo));
}

void SetDefaultMemoryValues()
{
	CommandResult result;
	result.command = ProtocolCommand::setDefaultMemoryValues;
	result.deviceId = Storage->readUint32tRecord(NVS_DEVICE_ID);
	result.status = true;
	result.message = "Default values loaded, please restart system";
	Storage->setDefaultValues();
	Wifi->sendTcpMessage(Formatter::reportMessageFromCommand(result));
}

void SaveWifiApRecord()
{
}

void RestartSystem()
{
	CommandResult result;
	result.command = ProtocolCommand::restartSystem;
	result.deviceId = Storage->readUint32tRecord(NVS_DEVICE_ID);
	result.status = true;
	result.message = "RESTARTING SYSTEM IN 3 SECONDS";
	logString(tag, result.message);
	Wifi->sendTcpMessage(Formatter::reportMessageFromCommand(result));
	for (uint8_t i = 0; i < 3; i++)
	{
		logString(tag, ".");
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
	Wifi->sendTcpMessage("Restarting");
	esp_restart();
}

void DeviceConfigApiStack(char *buffer)
{

	if (strlen(buffer) > 0)
	{
		switch (buffer[3] & 0xFF)
		{
		case ProtocolCommand::restartSystem: // 7B 00 7C 00 7C 7D
			RestartSystem();
			break;
		case ProtocolCommand::sendDeviceInfo: // 7B 00 7C 0B 7C 7D
			SendDeviceInfo();
			break;
		case ProtocolCommand::sendWifiApRecords: // 7B 00 7C 1B 7C 7D
			SendWifiApRecordsScanned();
			break;
		case ProtocolCommand::setDefaultMemoryValues: // 7B 00 7C 0C 7C 7D
			SetDefaultMemoryValues();
			break;
		case ProtocolCommand::saveWifiApRecord: // 7B 00 7C 2B 7C 7D
			SaveWifiApRecord();
			break;
		default:
			logString(tag, "Invalid Character");
			break;
		}
		memset(buffer, 0, TCP_RX_BUFFER_SIZE);
	}
}

void initObjects()
{
	Storage = new LocalStorage();
	Storage->setDefaultValues();

	Uart = new Uarts();
	Uart->logString = logString;
	Uart->logDword = logDword;
	Uart->logFloat = logFloat;
	if (Storage->readBooleanRecord(NVS_UART2_EN))
		Uart->uartInitializer(2);

	Gpio = new InputsOutputs();
	Gpio->logString = logString;
	Gpio->logDword = logDword;
	Gpio->logFloat = logFloat;
	Gpio->initBlink();

	Wifi = new WifiService();
	Wifi->logString = logString;
	Wifi->logDword = logDword;
	Wifi->logFloat = logFloat;

	WifiConfig wifiConfig = Storage->readWifiConfig();
	if (Wifi->init(wifiConfig))
	{
		appServerconfig.port = 1100;
		appServerconfig.callback = DeviceConfigApiStack;
		Wifi->createTcpServer(appServerconfig);
		if (wifiConfig.mode != WifiMode::Ap)
		{
			ApRecordList apRecords[MAXIMUM_SIZE_OF_SCAN_LIST];
			Wifi->scanWifiNetworks(apRecords);
		}
	}
}

extern "C" void app_main(void)
{
	logString(tag, "Go project!");
	initObjects();
}
