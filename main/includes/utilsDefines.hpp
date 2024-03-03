#include <GlobalDefines.hpp>
#include <InputsOutputs.hpp>
#include <UartsFunctions.hpp>
#include <WiFiService.hpp>
#include <WebApiConsumer.hpp>

typedef struct _DeviceInformation
{
    uint32_t deviceId;
    string versionApp;
    WifiConfig wifiConfig;
    ApiConfig WifiApiClient;

} DeviceInformation;

typedef struct _CommandResult
{
    uint32_t deviceId;
    ProtocolCommand command;
    bool status;
    string message;
} CommandResult;
