#include <GlobalDefines.hpp>
#include <InputsOutputs.hpp>
#include <UartsFunctions.hpp>
#include <WiFiService.hpp>

typedef struct _DeviceInformation
{
    uint32_t deviceId;
    string versionApp;
    WifiConfig wifiConfig;

} DeviceInformation;
