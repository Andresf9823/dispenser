#include <Formatter.hpp>

Formatter::Formatter(/* args */)
{
}

string Formatter::apRecordsList(ApRecordList *apNetworks, uint16_t apQuantity)
{
    string docString;
    DynamicJsonDocument doc(TCP_TX_BUFFER_SIZE);

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
    serializeJson(doc, docString);
    this->logString(this->tag, docString.c_str());
    return docString;
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