#include <TcpService.hpp>

TcpService::TcpService(/* args */)
{
}

void TcpService::do_retransmit(const int sock, bool isConnected)
{
    int len = 1;
    uint8_t rx_buffer[512];

    while (len > 0)
    {
        len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        if (len < 0)
        {
            ESP_LOGE("TCP TRANSMIT", "Error occurred during receiving: errno %d", errno);
        }
        else if (len == 0)
        {
            ESP_LOGW("TCP TRANSMIT", "Connection closed");
        }
        else
        {
            rx_buffer[len] = 0; // Null-terminate whatever is received and treat it like a string
            ESP_LOGI("TCP TRANSMIT", "Received %d bytes: %s", len, rx_buffer);

            // send() can return less bytes than supplied length.
            // Walk-around for robust implementation.
            int to_write = len;
            while (to_write > 0)
            {
                int written = send(sock, rx_buffer + (len - to_write), to_write, 0);
                if (written < 0)
                {
                    ESP_LOGE("TCP TRANSMIT", "Error occurred during sending: errno %d", errno);
                    // Failed to retransmit, giving up
                    return;
                }
                to_write -= written;
            }
        }
    }
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
    int sock = 0;
    bool isConnected = false;
    char addr_str[128];

    while (listening)
    {
        if (!isConnected)
        {
            struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
            socklen_t addr_len = sizeof(source_addr);
            ESP_LOGE("TCP SERVER TASK", "Waiting accept connections");
            sock = accept(listenSocket, (struct sockaddr *)&source_addr, &addr_len);
            ESP_LOGE("TCP SERVER TASK", "SOCKET: %d", sock);

            if (source_addr.ss_family == PF_INET)
            {
                memset(addr_str, 0, 128);
                inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
                ESP_LOGE("TCP SERVER TASK", "Socket accepted ip address: %s", addr_str);
            }
            isConnected = true;
        }
        else
        {
            struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
            socklen_t addr_len = sizeof(source_addr);
            sock = getpeername(listenSocket, (struct sockaddr *)&source_addr, &addr_len);
            memset(addr_str, 0, 128);
            inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
            ESP_LOGE("TCP SERVER TASK", "IP: %s", addr_str);
        }
        do_retransmit(sock, isConnected);
        if (sock < 0)
        {
            ESP_LOGE("TCP SERVER TASK", "Unable to accept connection: errno %d", errno);
            isConnected = false;
            shutdown(sock, 0);
            close(sock);
            ESP_LOGE("TCP SERVER TASK", " errno %d", errno);
        }
    }

    ESP_LOGE("TCP SERVER TASK", "Closing port: %d", socketPort);
    shutdown(listenSocket, 0);
    close(listenSocket);
    vTaskDelete(NULL);
}

void TcpService::CreateSocket(uint16_t port)
{
    xTaskCreate(serverTask, "TCP SERVER", 4096, (void *)port, 5, NULL);
}

TcpService::~TcpService()
{
}
