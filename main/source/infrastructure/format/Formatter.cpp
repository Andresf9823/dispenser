#include "Formatter.hpp"

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
        if (deviceInfo.wifiConfig.mode == WifiMode::Ap || deviceInfo.wifiConfig.mode == WifiMode::ApStation)
        {
            JsonObject accessPoint = wifi.createNestedObject("AccessPoint");
            accessPoint["Ip"] = ipAddressToString(deviceInfo.wifiConfig.ApConfig.ip.ip);
            accessPoint["Mask"] = ipAddressToString(deviceInfo.wifiConfig.ApConfig.ip.mask);
            accessPoint["Gateway"] = ipAddressToString(deviceInfo.wifiConfig.ApConfig.ip.gateway);
            accessPoint["Mac"] = macToString(deviceInfo.wifiConfig.ApConfig.mac, sizeof(deviceInfo.wifiConfig.ApConfig.mac));
            accessPoint["Ssid"] = deviceInfo.wifiConfig.ApConfig.ssid;
            accessPoint["Auth"] = deviceInfo.wifiConfig.ApConfig.authentication;
            accessPoint["Mode"] = deviceInfo.wifiConfig.mode;
            accessPoint["Pass"] = deviceInfo.wifiConfig.ApConfig.password;

        }

        if (deviceInfo.wifiConfig.mode == WifiMode::Station || deviceInfo.wifiConfig.mode == WifiMode::ApStation)
        {
            JsonObject station = wifi.createNestedObject("Station");
            station["Ip"] = ipAddressToString(deviceInfo.wifiConfig.StaConfig.ip.ip);
            station["Mask"] = ipAddressToString(deviceInfo.wifiConfig.StaConfig.ip.mask);
            station["Gateway"] = ipAddressToString(deviceInfo.wifiConfig.StaConfig.ip.gateway);
            station["Mac"] = macToString(deviceInfo.wifiConfig.StaConfig.mac, sizeof(deviceInfo.wifiConfig.StaConfig.mac));
            station["TargetMac"] = macToString(deviceInfo.wifiConfig.StaConfig.targetMac, sizeof(deviceInfo.wifiConfig.StaConfig.targetMac));
            station["Ssid"] = deviceInfo.wifiConfig.StaConfig.ssid;
            station["Auth"] = deviceInfo.wifiConfig.StaConfig.authentication;
            station["Mode"] = deviceInfo.wifiConfig.mode;
            station["Pass"] = deviceInfo.wifiConfig.StaConfig.password;
        }
    }

    serializeJson(doc, jsonString);
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
        data["Channel"] = apNetworks[i].primaryChannel;
        apItems.add(data);
    }
    serializeJson(doc, docString);
    return docString;
}

string Formatter::reportMessageFromCommand(CommandResult result)
{
    string docString;
    DynamicJsonDocument doc(TCP_TX_BUFFER_SIZE);

    doc["Id"] = result.command;
    doc["IdDevice"] = result.deviceId;
    doc["Status"] = result.status;
    JsonObject data = doc.createNestedObject("Data");
    data["Message"] = result.message;

    serializeJson(doc, docString);
    return docString;
}

void Formatter::stringToIpAddress(uint8_t *_ip, string rawIp)
{
    memset(_ip, 0, 4);
    const char *separator = ",";
    char *ip = (char *)rawIp.c_str();
    strlwr(ip);
    const char *token = strtok(ip, separator);
    for (uint8_t i = 0; i < 4; i++)
    {
        if (token != NULL)
        {
            _ip[i] = static_cast<uint8_t>(atoi(token)) & 0xFF;
            token = strtok(NULL, separator);
        }
    }
}

void Formatter::stringToMac(uint8_t *_mac, string rawMac)
{
    memset(_mac, 0, 6);
    const char *separator = ":";
    char *mac = (char *)rawMac.c_str();
    const char *token = strtok(mac, separator);

    for (uint8_t i = 0; i < 6; i++)
    {
        if (token != NULL)
        {
            /*Cast uint8_t from const char[].
            Base must be 16 becasuse type char uses 2 uint8_t or bytes*/
            _mac[i] = static_cast<uint8_t>(strtoul(token, nullptr, 16)) & 0xFF;
            token = strtok(NULL, separator);
        }
    }
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

string Formatter::macToString(uint8_t *macAddress, size_t macSize)
{
    string macString;
    if (macSize == 6)
    {
        char mac[18];
        memset(mac, 0, 18);
        sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
        macString = string(mac);
    }
    else
    {
        macString = "Invalid MAC size";
    }

    return macString;
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