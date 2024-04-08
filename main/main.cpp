#include <GlobalDefines.hpp>
#include <fileSystem/LocalStorage.hpp>

using namespace std;

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

string SendDeviceInfo()
{
	DeviceInformation deviceInfo;
	deviceInfo.deviceId = Storage->readDwordRecord(NVS_DEVICE_ID);
	deviceInfo.versionApp = VERSION_APP;
	deviceInfo.wifiConfig = Wifi->getConfig();
	return Formatter::deviceInformation(deviceInfo);
}

string SetDefaultMemoryValues()
{
	CommandResult result;
	result.command = ProtocolCommand::setDefaultMemoryValues;
	result.deviceId = Storage->readDwordRecord(NVS_DEVICE_ID);
	result.status = true;
	result.message = "Default values loaded, please restart system";
	Storage->setDefaultValues();
	return Formatter::reportMessageFromCommand(result);
}

string SaveWifiApRecord(uint8_t index, string password)
{
	CommandResult result;
	ApRecordList record = Wifi->getRecordScannned(index);
	Storage->saveStationTarget(record, password);
	Wifi->setStaMacTarget(record.mac);

	logString(tag, "AP RECORD SSID SAVED -> " + string(record.ssid));
	logString(tag, "AP RECORD RSSI SAVED -> " + to_string(record.rssi));
	logString(tag, "AP RECORD AUTH SAVED -> " + to_string(record.authMode));
	logString(tag, "AP RECORD TMAC SAVED ->" + Formatter::macToString(record.mac, 6));

	result.command = ProtocolCommand::saveWifiApRecord;
	result.deviceId = Storage->readDwordRecord(NVS_DEVICE_ID);
	result.status = true;
	result.message = "Ap record saved on local storage";
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
	result.deviceId = Storage->readDwordRecord(NVS_DEVICE_ID);
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
		break;
	}
	if (Storage->writeStringRecord(key, Formatter::macToString(mac, 6)))
		result.message = "Saved mac " + Storage->readStringRecord(key) + " on network interface " + to_string(mode);
	else
		result.message = "Mac " + Storage->readStringRecord(key) + " on network interface was not save" + to_string(mode);
	logString(tag, result.message);
	return Formatter::reportMessageFromCommand(result);
}

string GetDeviceConfiguration()
{
	CommandResult result;
	result.command = ProtocolCommand::getDeviceConfiguration;
	result.deviceId = Storage->readDwordRecord(NVS_DEVICE_ID);
	if (Wifi->httpGet("https://api.chucknorris.io/jokes/random"))
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
	result.deviceId = Storage->readDwordRecord(NVS_DEVICE_ID);
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
	result.deviceId = Storage->readDwordRecord(NVS_DEVICE_ID);
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
	if (strlen(buffer) > 0)
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
			return SaveWifiApRecord(buffer[5], "1140893813");
			break;
		default:
			logString(tag, "Invalid Character");
			return "Invalid Character";
			break;
		}
		memset(buffer, 0, sizeof(TCP_RX_BUFFER_SIZE));
	}
	return "";
}

void initObjects()
{
	Storage = new LocalStorage();

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
		TcpServerConfiguration appServer = {
			.port = 1100,
			.callback = DeviceConfigApiStack,
		};
		Wifi->createTcpServer(appServer);

		if (wifiConfig.mode == WifiMode::Ap)
		{
		}else
		{
			// ApRecordList apRecords[MAXIMUM_SIZE_OF_SCAN_LIST];
			// Wifi->scanWifiNetworks(apRecords);
		}
	}
}

extern "C" void app_main(void)
{
	logString(tag, ">>>>> HERE WE GO!!! <<<<<");
	initObjects();
}
