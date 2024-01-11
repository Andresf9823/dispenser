#include <TcpService.hpp>

TcpService::TcpService(/* args */)
{
}

void TcpService::TcpAppStack()
{
    if (strlen(tcpBuffer) > 0)
    {
        switch (tcpBuffer[3] & 0xFF)
        {
        case ProtocolCommand::sendWifiApRecords: // 7B 00 7C 1B 7C 7D
            this->SendWifiApRecordsScanned();
            break;
        default:
            ESP_LOGI(tag, "tcpBuffer %d", tcpBuffer[3]);
            break;
        }
        memset(tcpBuffer, 0, sizeof(tcpBuffer));
    }
}

void TcpService::SendTcpMessage(char *message)
{
}

void TcpService::do_retransmit(const int sock)
{
    int len;
    uint8_t rx_buffer[RX_BUFFER_SIZE];

    do
    {
        len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);

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
            rx_buffer[len] = 0; // Null-terminate whatever is received and treat it like a string

            char data[sizeof(rx_buffer) * sizeof(char)];
            memset(data, 0, sizeof(data));

            uint k = 0;
            for (uint i = 0; i < len; i++)
            {
                k += sprintf(data + k, "%02X ", rx_buffer[i]);
            }

            ESP_LOGI("TCP retransmit", "Received %d bytes: %s", len, (const char *)data);

            if (isValidFrame(rx_buffer, len))
            {
                memset(tcpBuffer, 0, sizeof(tcpBuffer));
                memcpy(tcpBuffer, (const char *)rx_buffer, sizeof(data));
            }

            // send() can return less bytes than supplied length.
            // Walk-around for robust implementation.
            // int to_write = len;
            // while (to_write > 0)
            // {
            //     int written = send(sock, rx_buffer + (len - to_write), to_write, 0);
            //     if (written < 0)
            //     {
            //         ESP_LOGE("TCP retransmit", "Error occurred during sending: errno %d", errno);
            //         // Failed to retransmit, giving up
            //         return;
            //     }
            //     to_write -= written;
            // }
        }
    } while (len > 0);
}

bool TcpService::isValidFrame(uint8_t *buffer, uint len)
{
    bool begin, finish = false;
    for (uint i = 0; i < len; i++)
    {
        if (buffer[i] == (uint8_t)'{')
            begin = true;
        if (buffer[i] == (uint8_t)'}')
            finish = true;
        if (begin && finish)
        {
            ESP_LOGI(tag, "%s", "Valid frame");
            return true;
        }
    }
    ESP_LOGE(tag, "%s", "Invalid frame");
    return false;
}

void TcpService::serverTask(void *pvParameters)
{
    int opt = 1;
    int addressFamily = AF_INET;
    int ipProtocol = 0;
    int socketPort = (int)pvParameters;
    bool listening = true;
    struct sockaddr_storage destinationAddress;

    struct sockaddr_in *destinationAddressIpv4 = (struct sockaddr_in *)&destinationAddress;
    destinationAddressIpv4->sin_addr.s_addr = htonl(INADDR_ANY);
    destinationAddressIpv4->sin_family = AF_INET;
    destinationAddressIpv4->sin_port = htons(socketPort);
    ipProtocol = IPPROTO_IP;

    int listenSocket = socket(addressFamily, SOCK_STREAM, ipProtocol);
    if (listenSocket < 0)
    {
        ESP_LOGE("TCP SERVER TASK", "Unable to create socket in port: %d ", socketPort);
        vTaskDelete(NULL);
        return;
    }

    ESP_LOGI("TCP SERVER TASK", "Socket created");

    int err = bind(listenSocket, (struct sockaddr *)&destinationAddress, sizeof(destinationAddress));
    if (err != 0)
    {
        ESP_LOGE("TCP SERVER TASK", "Socket unable to bind: errno %d", errno);
        ESP_LOGE("TCP SERVER TASK", "IPPROTO: %d", addressFamily);
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
        ESP_LOGI("TCP SERVER", "Socket listening");

        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t addr_len = sizeof(source_addr);
        int sock = accept(listenSocket, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0)
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
        if (source_addr.ss_family == PF_INET)
        {
            inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
        }
        ESP_LOGI("TCP SERVER", "Socket accepted ip address: %s", addr_str);

        do_retransmit(sock);

        shutdown(sock, 0);
        close(sock);
    }

    ESP_LOGE("TCP SERVER TASK", "Closing port: %d", socketPort);
    shutdown(listenSocket, 0);
    close(listenSocket);
    vTaskDelete(NULL);
}

void TcpService::CreateSocket(uint16_t port)
{
    xTaskCreate(serverTask, "TCP SERVER", TCP_TASK_SIZE, (uint16_t *)port, 5, NULL);
}

TcpService::~TcpService()
{
}
