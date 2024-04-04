#ifndef _FORMATTER_HPP_
#define _FORMATTER_HPP_

#include <GlobalDefines.hpp>
#include "utilsDefines.hpp"
#include "ArduinoJson-v6.21.3.h"

using namespace std;

class Formatter
{
private:
    static constexpr string tag = "FORMATTER";

public:
    Formatter();
    static void jsonToCharArray(DynamicJsonDocument doc, char *bufferIn);
    static void stringToIpAddress(uint8_t *ip, string rawIp);
    static void stringToMac(uint8_t *_mac, string rawMac);
    static string reportMessageFromCommand(CommandResult result);
    static string ipAddressToString(uint8_t *ipAddress);
    static string macToString(uint8_t *macAddress, uint8_t macSize);
    static string deviceInformation(DeviceInformation deviceInfo);
    static string apRecordsList(ApRecordList *apNetworks, uint16_t apQuantity);
    static DynamicJsonDocument charArrayToJson(char *bufferIn);
    ~Formatter();
};

#endif