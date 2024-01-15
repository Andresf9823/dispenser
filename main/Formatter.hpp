#ifndef _FORMATTER_HPP_
#define _FORMATTER_HPP_

#include <GlobalDefines.hpp>
#include <WiFiService.hpp>
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
    DynamicJsonDocument charArrayToJson(char *bufferIn);
    string apRecordsList(ApRecordList *apNetworks, uint16_t apQuantity);
    void (*logString)(string TAG, string message);
    void (*logDword)(string TAG, int32_t logNumber);
    void (*logFloat)(string TAG, double logFloating);
    ~Formatter();
};

#endif