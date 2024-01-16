#include <Formatter.hpp>

Formatter::Formatter(/* args */)
{
}

string Formatter::deviceInformation(DeviceInformation deviceInfo)
{
    string jsonString;
    DynamicJsonDocument doc(TCP_TX_BUFFER_SIZE);

    doc["Id"] = ProtocolCommand::sendDeviceInfo;
    doc["IdDevice"] = deviceInfo.deviceId;
    doc["Version"] = deviceInfo.versionApp;
    doc["WifiMode"] = deviceInfo.wifiConfig.mode;

    if (deviceInfo.wifiConfig.mode != 0)
    {
        JsonObject wifi = doc.createNestedObject("Wifi");
        if (deviceInfo.wifiConfig.mode == WiFiMode::Ap || deviceInfo.wifiConfig.mode == WiFiMode::ApStation)
        {
            JsonObject accessPoint = wifi.createNestedObject("AccessPoint");
            accessPoint["Ip"] = ipAddressToString(deviceInfo.wifiConfig.ApConfig.ip);
            accessPoint["mask"] = ipAddressToString(deviceInfo.wifiConfig.ApConfig.mask);
            accessPoint["Gateway"] = ipAddressToString(deviceInfo.wifiConfig.ApConfig.gateway);
            accessPoint["Mac"] = macToString(deviceInfo.wifiConfig.ApConfig.mac, sizeof(deviceInfo.wifiConfig.ApConfig.mac));
            accessPoint["Ssid"] = deviceInfo.wifiConfig.ApConfig.ssid;
            accessPoint["Auth"] = deviceInfo.wifiConfig.ApConfig.auth;
            accessPoint["Mode"] = deviceInfo.wifiConfig.ApConfig.mode;
            accessPoint["Pass"] = deviceInfo.wifiConfig.ApConfig.password;
        }

        if (deviceInfo.wifiConfig.mode == WiFiMode::Station || deviceInfo.wifiConfig.mode == WiFiMode::ApStation)
        {
            JsonObject station = wifi.createNestedObject("Station");
            station["Ip"] = ipAddressToString(deviceInfo.wifiConfig.StaConfig.ip);
            station["mask"] = ipAddressToString(deviceInfo.wifiConfig.StaConfig.mask);
            station["Gateway"] = ipAddressToString(deviceInfo.wifiConfig.StaConfig.gateway);
            station["Mac"] = macToString(deviceInfo.wifiConfig.StaConfig.mac, sizeof(deviceInfo.wifiConfig.StaConfig.mac));
            station["Ssid"] = deviceInfo.wifiConfig.StaConfig.ssid;
            station["Auth"] = deviceInfo.wifiConfig.StaConfig.auth;
            station["Mode"] = deviceInfo.wifiConfig.StaConfig.mode;
            station["Pass"] = deviceInfo.wifiConfig.StaConfig.password;
        }
    }

    serializeJson(doc, jsonString);
    logString(tag, jsonString);
    return jsonString;
}

string Formatter::apRecordsList(ApRecordList *apNetworks, uint16_t apQuantity)
{
    string docString;
    DynamicJsonDocument doc(TCP_TX_BUFFER_SIZE);

    doc["Id"] = ProtocolCommand::sendWifiApRecords;
    doc["Quantity"] = apQuantity;
    JsonArray apItems = doc.createNestedArray("Records");
    for (uint16_t i = 0; i < apQuantity; i++)
    {
        DynamicJsonDocument data(128);
        data["Mac"] = macToString(apNetworks[i].mac, sizeof(apNetworks[i].mac));
        data["Ssid"] = apNetworks[i].ssid;
        data["Authmode"] = apNetworks[i].authMode;
        data["Rssi"] = apNetworks[i].rssi;
        apItems.add(data);
    }
    serializeJson(doc, docString);
    logString(tag, docString);
    return docString;
}

string Formatter::ipAddressToString(uint8_t *ipAddress)
{
    string ipString;
    if (sizeof(ipAddress) == 4)
    {
        char ip[16];
        memset(ip, 0, 16);
        sprintf(ip, "%d,%d,%d,%d", ipAddress[0], ipAddress[1], ipAddress[2], ipAddress[3]);
        ipString = string(ip);
    }
    else
    {
        ipString = "Invalid IpAddress size";
    }

    return ipString;
}

string Formatter::macToString(uint8_t *macAddress, uint8_t macSize)
{
    string ipString;
    if (macSize == 6)
    {
        char mac[18];
        memset(mac, 0, 18);
        sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
        ipString = string(mac);
    }
    else
    {
        ipString = "Invalid MAC size";
    }

    return ipString;
}

void Formatter::jsonToCharArray(DynamicJsonDocument doc, char *buffer)
{
    serializeJson(doc, buffer, sizeof(buffer));
}

DynamicJsonDocument Formatter::charArrayToJson(char *bufferIn)
{
    DynamicJsonDocument doc(sizeof(bufferIn));
    deserializeJson(doc, bufferIn);
    return doc;
}

Formatter::~Formatter()
{
}