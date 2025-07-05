#include "TcpClient.hpp"

void TcpClient::connectToServer(IpClientConfiguration &config)
{
   //Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        ESP_LOGE(tag.c_str(), "Socket creation failed: %d", errno);
        return;
    }

    // Set up the server address structure
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(config.port);
    inet_pton(AF_INET, (char *)config.ipTarget, &server_addr.sin_addr);

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        ESP_LOGE(tag.c_str(), "Connection failed: %d", errno);
        close(sock);
        return;
    }

    ESP_LOGI(tag.c_str(), "Connected to server at %d.%d.%d.%d:%d", config.ipTarget[0], config.ipTarget[1], config.ipTarget[2], config.ipTarget[3], config.port);

    //Send data to the server using the callback function
    char dataToSend[256] = "Hello from TCP client!";
    string response = config.callback(dataToSend);

    //Close the socket after use
    close(sock);
}