#include "TcpServer.hpp"

void TcpServer::sendMessage(string &message)
{
    if (this->socketState != 0)
    {
        // send() can return less bytes than supplied length.
        // Walk-around for robust implementation.
        int to_write = strlen(message.c_str());
        int len = strlen(message.c_str());
        while (to_write > 0)
        {
            int written = send(this->socketState, message.c_str() + (len - to_write), to_write, 0);
            if (written < 0)
            {
                ESP_LOGE("TCP retransmit", "Error occurred during sending: errno %d", errno);
                // Failed to retransmit, giving up
                return;
            }
            to_write -= written;
        }
    }
    else
    {
        ESP_LOGW(tag.c_str(), "%s", "Socket is not connected");
    }
}

void TcpServer::serverTask(string (*callbackFunction)(char *))
{
    int len;
    char rxTcpbuffer[TCP_RX_BUFFER_SIZE];

    do
    {
        len = recv(this->socketState, rxTcpbuffer, sizeof(rxTcpbuffer) - 1, 0);
        if (len < 0)
        {
            ESP_LOGE("TCP retransmit", "Error occurred during receiving: errno %d", errno);
        }
        else if (len == 0)
        {
            ESP_LOGW("TCP retransmit", "Connection closed");
        }
        else
        {
            rxTcpbuffer[len] = 0; // Null-terminate whatever is received and treat it like a string
            char data[sizeof(rxTcpbuffer) * sizeof(char)];
            memset(data, 0, sizeof(data));

            uint k = 0;
            for (uint i = 0; i < len; i++)
            {
                k += sprintf(data + k, "%02X ", rxTcpbuffer[i]);
            }
            ESP_LOGI("TCP retransmit", "Received %d bytes: %s", len, (const char *)data);

            if (isValidFrame(rxTcpbuffer, len))
            {
                string data_ = callbackFunction(rxTcpbuffer);
                if (!data_.empty())
                {
                    this->sendMessage(data_);
                }
            }
            memset(rxTcpbuffer, 0, TCP_RX_BUFFER_SIZE);
        }
    } while (len > 0);
}

void TcpServer::serverLaunch(void *pvParameters)
{
    int opt = 1;
    IpServerConfiguration *instance = static_cast<IpServerConfiguration *>(pvParameters);
    ESP_LOGW("TCP SERVICE", "%i", instance->port);
    bool listening = true;
    struct sockaddr_in destinationAddress;
    destinationAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    destinationAddress.sin_family = AF_INET;
    destinationAddress.sin_port = htons(instance->port);

    int listenSocket = socket(destinationAddress.sin_family, SOCK_STREAM, IPPROTO_IP);
    if (listenSocket < 0)
    {
        ESP_LOGE("TCP SERVER TASK", "Unable to create socket in port: %d ", instance->port);
        vTaskDelete(NULL);
        return;
    }

    ESP_LOGI("TCP SERVER TASK", "Socket created");

    int err = bind(listenSocket, (struct sockaddr *)&destinationAddress, sizeof(destinationAddress));
    if (err != 0)
    {
        ESP_LOGE("TCP SERVER TASK", "Socket unable to bind: errno %d", errno);
        ESP_LOGE("TCP SERVER TASK", "IPPROTO: %d", destinationAddress.sin_family);
        listening = false;
    }

    err = listen(listenSocket, 1);
    if (err != 0)
    {
        ESP_LOGE("TCP SERVER TASK", "Error occurred during listen: errno %d", errno);
        listening = false;
    }

    setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    char addr_str[128];

    while (listening)
    {
        ESP_LOGW("TCP SERVER", "Socket listening on port %d", instance->port);
        struct sockaddr_storage * source_addr; // Large enough for both IPv4 or IPv6
        socklen_t addr_len = sizeof(source_addr);
        this->socketState = accept(listenSocket, (struct sockaddr *)&source_addr, &addr_len);
        if (this->socketState < 0)
        {
            ESP_LOGE("TCP SERVER", "Unable to accept connection: errno %d", errno);
            break;
        }

        // Set tcp keepalive option
        // setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(int));
        // setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(int));
        // setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(int));
        // setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(int));

        // Convert ip address to string
        if (source_addr->ss_family == PF_INET)
        {
            inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
        }
        ESP_LOGW("TCP SERVER", "Socket accepted ip address: %s", addr_str);
        serverTask(instance->callback);
        shutdown(this->socketState, 0);
        close(this->socketState);
    }

    ESP_LOGE("TCP SERVER TASK", "Closing port: %d", instance->port);
    shutdown(listenSocket, 0);
    close(listenSocket);
    vTaskDelete(NULL);
}

bool TcpServer::isValidFrame(char *buffer, uint len)
{
    if ((buffer[0] == (uint8_t)'{') && (buffer[len - 1] == (uint8_t)'}'))
        return true;
    ESP_LOGE("Incoming frame validation", "%s", "Invalid frame");
    return false;
}

void TcpServer::createServer(IpServerConfiguration &config)
{
    xTaskCreatePinnedToCore([](void *pvParameters)
                            {
        TcpServer *tcpServer = static_cast<TcpServer *>(pvParameters);
        tcpServer->serverLaunch(pvParameters); }, "TCP SERVER", TCP_TASK_SIZE, &config, 5, NULL, 0);
}