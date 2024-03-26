#include <domain/database/Formatter.hpp>

Formatter::Formatter(/* args */)
{
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

string Formatter::jsonToString(DynamicJsonDocument doc)
{
    string buffer;
    // serializeJson(doc, buffer, sizeof(buffer));
    return buffer;
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