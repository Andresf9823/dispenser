#include <GlobalDefines.hpp>
#include <fileSystem/LocalStorage.hpp>

using namespace std;

static string tag = "MAIN";

uint32_t DEVICE_ID;
TcpServerConfiguration appServer;

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

string SendDeviceInfo()
{
	DeviceInformation deviceInfo;
	deviceInfo.deviceId = DEVICE_ID;
	deviceInfo.versionApp = VERSION_APP;
	deviceInfo.wifiConfig = Wifi->getConfig();
	return Formatter::deviceInformation(deviceInfo);
}

string SetDefaultMemoryValues()
{
	CommandResult result;
	result.command = ProtocolCommand::setDefaultMemoryValues;
	result.deviceId = DEVICE_ID;
	result.status = true;
	result.message = "Default values loaded, please restart system";
	Storage->setDefaultValues();
	return Formatter::reportMessageFromCommand(result);
}

string SaveWifiApRecord(uint8_t index, string password)
{
	CommandResult result;
	result.command = ProtocolCommand::saveWifiApRecord;
	result.deviceId = DEVICE_ID;
	ApRecordList record = Wifi->getRecordScannned(index);
	if (string(record.ssid).empty())
	{
		string message = "Target configuration not saved: WiFi network scanning has not been performed or index " + to_string(index) + " doesn't exist.";
		logString(tag, message);
		result.status = false;
		result.message = message;
		return Formatter::reportMessageFromCommand(result);
	}
	Storage->saveStationTarget(record, password);
	// Wifi->setStationConfig(Storage->readWifiConfig());

	logString(tag, "AP RECORD SSID SAVED -> " + string(record.ssid));
	logString(tag, "AP RECORD RSSI SAVED -> " + to_string(record.rssi));
	logString(tag, "AP RECORD AUTH SAVED -> " + to_string(record.authMode));
	logString(tag, "AP RECORD CHAN SAVED -> " + to_string(record.primaryChannel));
	logString(tag, "AP RECORD TMAC SAVED -> " + Formatter::macToString(record.mac, 6));

	result.status = true;
	result.message = "Ap record saved on local storage";
	return Formatter::reportMessageFromCommand(result);
}

string SetWifiMode(WifiMode mode)
{
	CommandResult result;
	result.deviceId = DEVICE_ID;
	result.command = ProtocolCommand::setWifiMode;
	result.status = false;
	if (mode == WifiMode::Ap || mode == WifiMode::Station || mode == WifiMode::ApStation)
	{
		bool saved = Storage->writeByteRecord(NVS_WIFI_MODE, mode);
		result.message = "Wifi mode " + to_string(mode) + (saved ? " saved succesfully." : " not saved.");
	}
	else
	{
		result.message = "Wifi mode don't exist";
	}
	return Formatter::reportMessageFromCommand(result);
}

string SendWifiApRecordsScanned()
{
	ApRecordList apRecords[MAXIMUM_SIZE_OF_SCAN_LIST];
	uint16_t recordsScanned = Wifi->scanWifiNetworks(apRecords);
	return Formatter::apRecordsList(apRecords, recordsScanned);
}

string SetMac(uint8_t *mac, WifiMode mode)
{
	CommandResult result;
	result.deviceId = DEVICE_ID;
	result.status = true;
	string key;
	switch (mode)
	{
	case WifiMode::Ap:
		key = NVS_AP_MAC;
		break;
	case WifiMode::Station:
		key = NVS_STA_MAC;
		break;
	case WifiMode::Unkown: // Ethernet interface
		key = NVS_ETH_MAC;
		break;
	default:
		result.status = false;
		break;
	}
	if (Storage->writeStringRecord(key, Formatter::macToString(mac, 6)))
		result.message = "Saved mac " + Storage->readStringRecord(key) + " on network interface " + to_string(mode);
	else
	{
		result.status = false;
		result.message = "Mac " + Storage->readStringRecord(key) + " on network interface" + to_string(mode) + " not saved" + to_string(mode);
	}
	logString(tag, result.message);
	return Formatter::reportMessageFromCommand(result);
}

string GetDeviceConfiguration()
{
	logString(tag, "GetDeviceConfiguration");
	CommandResult result;
	result.command = ProtocolCommand::getDeviceConfiguration;
	result.deviceId = DEVICE_ID;
	if (Wifi->httpGet("https://rickandmortyapi.com/api/character/74"))
	{
		result.message = "Device configuration downloaded";
		result.status = true;
	}
	else
	{
		result.message = "Device configuration not downloaded";
		result.status = false;
	}
	return Formatter::reportMessageFromCommand(result);
}

string RestartSystem()
{
	CommandResult result;
	result.command = ProtocolCommand::restartSystem;
	result.deviceId = DEVICE_ID;
	result.status = true;
	result.message = "RESTARTING SYSTEM IN 3 SECONDS";
	logString(tag, result.message);
	for (uint8_t i = 0; i < 3; i++)
	{
		logString(tag, ".");
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
	logString(tag, "Restarting");
	esp_restart();
	return Formatter::reportMessageFromCommand(result);
}

string LoginApp(string userEmail, string password)
{
	CommandResult result;
	result.command = ProtocolCommand::login;
	result.deviceId = DEVICE_ID;
	if (userEmail == "andresf9806@gmail.com" && password == "Chan.61522_")
	{
		result.status = true;
		result.message = "User " + userEmail + " logged succedfully";
	}
	else
	{
		result.status = false;
		result.message = "Please check the credentials";
	}
	return Formatter::reportMessageFromCommand(result);
}

string DeviceConfigApiStack(char *buffer)
{
	switch (buffer[3] & 0xFF)
	{
	case ProtocolCommand::restartSystem: // 7B 00 7C 00 7C 7D
		return RestartSystem();
		break;
	case ProtocolCommand::login: // 7B 00 7C 01 7C 7D
		return LoginApp("andresf9806@gmail.com", "Chan.61522_");
		break;
	case ProtocolCommand::sendDeviceInfo: // 7B 00 7C 0B 7C 7D
		return SendDeviceInfo();
		break;
	case ProtocolCommand::sendWifiApRecords: // 7B 00 7C 1B 7C 7D
		return SendWifiApRecordsScanned();
	case ProtocolCommand::setWifiMode: // 7B 00 7C 3B 7C 03 7C 7D
		return SetWifiMode(WifiMode(buffer[5]));
		break;
	case ProtocolCommand::setDefaultMemoryValues: // 7B 00 7C 0C 7C 7D
		return SetDefaultMemoryValues();
		break;
	case ProtocolCommand::getDeviceConfiguration: // 7B 00 7C 1C 7C 7D
		return GetDeviceConfiguration();
		break;
	case ProtocolCommand::setMac: // 7B 00 7C 2C 7C 01 7C 00 A3 01 10 00 10 7C 7D
		uint8_t mac[6];
		for (uint8_t i = 0; i < 6; i++)
			mac[i] = (uint8_t)buffer[7 + i];
		return SetMac(mac, (WifiMode)buffer[5]);
		break;
	case ProtocolCommand::saveWifiApRecord: // 7B 00 7C 2B 7C 00 7C 7D
		char password[32];
		memset(password, 0, sizeof(password));
		if (buffer[6] == 0x7C)
			for (uint8_t i = 0; i < sizeof(password); i++)
			{
				if (buffer[7 + i] == 0x7C)
					break;
				password[i] = buffer[7 + i];
			}

		return SaveWifiApRecord(buffer[5], string(password));
		break;
	default:
		logString(tag, "Invalid Character");
		return "Invalid Character";
		break;
	}
	memset(buffer, 0, sizeof(TCP_RX_BUFFER_SIZE));
}

void initObjects()
{
	Storage = new LocalStorage();
	// Storage->setDefaultValues();
	DEVICE_ID = Storage->readDwordRecord(NVS_DEVICE_ID);

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
		appServer.port = 1100;
		appServer.callback = &DeviceConfigApiStack;
		Wifi->createTcpServer(appServer);
	}
}

extern "C" void app_main(void)
{
	logString(tag, ">>>>> HERE WE GO!!! <<<<<");
	initObjects();
}
