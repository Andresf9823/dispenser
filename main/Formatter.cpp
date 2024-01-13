#include <Formatter.hpp>

Formatter::Formatter(/* args */)
{
}

DynamicJsonDocument Formatter::apRecordsListToJson(ApRecordList *apNetworks, uint16_t apQuantity)
{
    char docString[1200];
    DynamicJsonDocument doc(sizeof(docString));

    doc["Id"] = 6;
    doc["Quantity"] = apQuantity;
    JsonArray apItems = doc.createNestedArray("Records");
    for (uint16_t i = 0; i < apQuantity; i++)
    {
        DynamicJsonDocument data(128);
        data["Mac"] = apNetworks[i].mac;
        data["Ssid"] = apNetworks[i].ssid;
        data["Authmode"] = apNetworks[i].authMode;
        data["Rssi"] = apNetworks[i].rssi;
        apItems.add(data);
    }
    serializeJson(doc, docString, sizeof(docString));
    this->logString(this->tag, (const char *)docString);
    return doc;
}

Formatter::~Formatter()
{
}