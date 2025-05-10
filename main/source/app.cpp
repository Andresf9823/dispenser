#include "app.hpp"

// Función callback para el servidor TCP
string helloWorldCallback(char *bufferIn)
{
    return "Hola Mundo";
}

string byeWorldCallback(char *bufferIn)
{
    return "Bye Mundo";
}

App::App()
{
    try
    {
        ESP_LOGW(this->tag.c_str(), "%s", ">>>>>>>>>>>>> APP STARTED <<<<<<<<<<<<<<");

        unique_ptr<LocalStorage> localStorage = make_unique<LocalStorage>();

        // Configurar y crear servidor TCP

        WifiConfig wifiConfig = localStorage->loadWifiConfig();
        unique_ptr<WifiService> wifiService = make_unique<WifiService>();
        if (wifiService->init(wifiConfig))
        {
            ESP_LOGW(this->tag.c_str(), "%s", ">>>>>>>>>>>>> WIFI INITIALIZED <<<<<<<<<<<<<<");

            IpServerConfiguration tcpServerConfig;
            tcpServerConfig.port = 8080;
            tcpServerConfig.type = IpServerType::TCP;
            tcpServerConfig.callback = helloWorldCallback;
            wifiService->createIpServer(tcpServerConfig);

            IpClientConfiguration tcpClientConfig;
            tcpClientConfig.port = 1100;
            tcpClientConfig.type = IpServerType::TCP;
            tcpClientConfig.ipTarget[0] = 192;
            tcpClientConfig.ipTarget[1] = 168;
            tcpClientConfig.ipTarget[2] = 1;
            tcpClientConfig.ipTarget[3] = 134;
            tcpClientConfig.callback = byeWorldCallback;
            wifiService->createIpClient(tcpClientConfig);
        }
    }
    catch (const std::exception &e)
    {
        ESP_LOGE(this->tag.c_str(), "%s", e.what());
    }
}