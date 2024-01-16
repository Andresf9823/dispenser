#ifndef _FORMATTER_HPP_
#define _FORMATTER_HPP_

#include <GlobalDefines.hpp>
#include <utilsDefines.hpp>
#include <ArduinoJson-v6.21.3.h>

using namespace std;

class Formatter
{
private:
    /* data */
public:
    Formatter(/* args */);
    static constexpr string tag = "FORMATTER";
    void jsonToCharArray(DynamicJsonDocument doc, char *bufferIn);
    string ipAddressToString(uint8_t *ipAddress);
    string macToString(uint8_t *macAddress, uint8_t macSize);
    string deviceInformation(DeviceInformation deviceInfo);
    string apRecordsList(ApRecordList *apNetworks, uint16_t apQuantity);
    DynamicJsonDocument charArrayToJson(char *bufferIn);
    void (*logString)(string TAG, string message);
    void (*logDword)(string TAG, int32_t logNumber);
    void (*logFloat)(string TAG, double logFloating);
    ~Formatter();
};

#endif