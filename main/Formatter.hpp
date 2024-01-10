#ifndef _FORMATTER_HPP_
#define _FORMATTER_HPP_

#include <GlobalDefines.hpp>
#include <WiFiService.hpp>
#include <ArduinoJson-v6.21.3.h>

class Formatter
{
private:
    /* data */
public:
    Formatter(/* args */);
    const char *tag = "FORMATTER";
    DynamicJsonDocument apRecordsListToJson(ApRecordList *apNetworks, uint16_t apQuantity);
    void (*logString)(const char *TAG, const char *message);
    void (*logDword)(const char *TAG, uint32_t logNumber);
    void (*logFloat)(const char *TAG, double logFloating);
    ~Formatter();
};

#endif