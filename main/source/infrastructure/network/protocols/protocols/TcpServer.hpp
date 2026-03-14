#ifndef _TCP_HPP_
#define _TCP_HPP_

#include <GlobalDefines.hpp>
#include <freertos/FreeRTOS.h>
#include "freertos/event_groups.h"
#include <freertos/task.h>
#include <freertos/timers.h>

#include "../ProtocolRepository.hpp"

using namespace std;

#define TCP_RX_BUFFER_SIZE (KB) / 2
#define TCP_TX_BUFFER_SIZE (KB) * (1.5)
#define TCP_TASK_SIZE (KB) * (4)
#define TCP_MAX_SERVERS 5

class TcpServer : public IpServerRepository
{
private:
    static constexpr const string tag = "TCP SERVER";
    static bool isValidFrame(char *frame, uint len);
    void sendMessage(string &message);
    void serverLaunch(void *pvParameters);
    void serverTask(function<string(char *dataToSend)> callbackFunction);
    void cleanUpServer(int &listenSocket);
    int socketState = -1;
    IpServerConfiguration serverConfig;

    struct TcpServerTaskArgs
    {
        TcpServer *server;
        IpServerConfiguration *config;
    };

public:
    TcpServer() = default;
    void createServer(IpServerConfiguration &config) override;
    ~TcpServer() = default;
};
#endif