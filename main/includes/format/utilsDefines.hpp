#include <GlobalDefines.hpp>
#include <gpio/Gpio.hpp>
#include <wire/Uarts.hpp>
#include <network/wifi/Wifi.hpp>
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
