#include "TcpServer.hpp"

/**
 * Sends a message through the connected TCP socket.
 * If the socket is not connected, logs a warning and exits.
 * Handles sending in fragments if the message is long.
 * @param message The message string to send.
 */
void TcpServer::sendMessage(string &message)
{
    if (this->socketState < 0)
    {
        ESP_LOGW(tag.c_str(), "%s", "Socket is not connected");
        return;
    }
    // send() can return less bytes than supplied length.
    // Walk-around for robust implementation.
    int to_write = message.length();
    int len = to_write;
    if (len > TCP_TX_BUFFER_SIZE)
    {
        ESP_LOGE(tag.c_str(), "Message too long to send: %d bytes", len);
        return;
    }
    if (len <= 0)
    {
        ESP_LOGE(tag.c_str(), "Nothing to send");
        return;
    }

    const char *message_ = message.c_str();
    while (to_write > 0)
    {
        int written = send(this->socketState, message_ + (len - to_write), to_write, 0);
        if (written < 0)
        {
            ESP_LOGE("TCP MESSAGE", "Error occurred during sending: errno %d", errno);
            // Failed to retransmit, giving up
            return;
        }
        to_write -= written;
    }
}

/**
 * Main task for receiving data from the TCP client.
 * Continuously listens for data on the socket, validates frames, and calls a callback if valid.
 * Handles connection errors and closes the socket if necessary.
 * @param callbackFunction Callback function that processes received data and returns a response.
 */
void TcpServer::serverTask(function<string(char *dataToSend)> callbackFunction)
{
    int len;
    char rxTcpBuffer[TCP_RX_BUFFER_SIZE];

    do
    {
        if (this->socketState < 0)
        {
            ESP_LOGW("TCP retransmit", "Socket invalid (socketState=%d), stopping receive loop", this->socketState);
            break;
        }

        memset(rxTcpBuffer, 0, sizeof(rxTcpBuffer));
        ESP_LOGD("TCP retransmit", "Waiting to recv on socket %d", this->socketState);
        len = recv(this->socketState, rxTcpBuffer, sizeof(rxTcpBuffer) - 1, 0);

        if (len < 0)
        {
            // Map common recoverable / connection-close errors to non-fatal flows.
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
            {
                ESP_LOGD("TCP retransmit", "recv() would block or was interrupted (errno %d: %s), retrying", errno, strerror(errno));
                continue; // Retry receiving
            }

            if (errno == ECONNRESET || errno == ENOTCONN || errno == EPIPE)
            {
                ESP_LOGW("TCP retransmit", "Connection closed by peer (errno %d: %s)", errno, strerror(errno));
                this->cleanUpServer(this->socketState);
                break;
            }

            // All other errors are unexpected.
            ESP_LOGE("TCP retransmit", "Error occurred during receiving: errno %d (%s)", errno, strerror(errno));
            break;
        }
        if (len == 0)
        {
            ESP_LOGW("TCP retransmit", "Connection closed");
            this->cleanUpServer(this->socketState);
            break;
        }
        rxTcpBuffer[len] = 0; // Null-terminate whatever is received and treat it like a string
        char data[sizeof(rxTcpBuffer)];
        memset(data, 0, sizeof(data));

        uint k = 0;
        for (uint i = 0; i < len; i++)
        {
            k += sprintf(data + k, "%02X ", rxTcpBuffer[i]);
        }

        ESP_LOGI("TCP retransmit", "Received %d bytes: %s", len, (const char *)data);

        if (isValidFrame(rxTcpBuffer, len))
        {
            string data = callbackFunction(rxTcpBuffer);
            if (!data.empty())
                ESP_LOGW("DATA", "%s", data.c_str());
        }
    } while (len > 0);
}

/**
 * Starts the TCP server: creates the socket, configures it, listens for connections, and accepts clients.
 * For each accepted client, configures keepalive and launches the receive task.
 * @param pvParameters Pointer to the server configuration (IpServerConfiguration).
 */
void TcpServer::serverLaunch(void *pvParameters)
{
    IpServerConfiguration *instance = static_cast<IpServerConfiguration *>(pvParameters);
    int opt = 1;
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
        this->cleanUpServer(listenSocket);
    }

    ESP_LOGW("TCP SERVER TASK", "Socket created");

    int err = bind(listenSocket, (struct sockaddr *)&destinationAddress, sizeof(destinationAddress));
    if (err != 0)
    {
        ESP_LOGE("TCP SERVER TASK", "Socket unable to bind: errno %d", errno);
        ESP_LOGE("TCP SERVER TASK", "IPPROTO: %d", destinationAddress.sin_family);
        listening = false;
        this->cleanUpServer(listenSocket);
    }

    err = listen(listenSocket, 1);
    if (err != 0)
    {
        ESP_LOGE("TCP SERVER TASK", "Error occurred during listen: errno %d", errno);
        listening = false;
        this->cleanUpServer(listenSocket);
    }

    setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    char addr_str[128];

    while (listening)
    {
        ESP_LOGW("TCP SERVER", "Socket listening on port %d", instance->port);
        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t addr_len = sizeof(source_addr);
        this->socketState = accept(listenSocket, (struct sockaddr *)&source_addr, &addr_len);
        if (this->socketState < 0)
        {
            ESP_LOGE("TCP SERVER", "Unable to accept connection: errno %d", errno);
            break;
        }

        // Set tcp keepalive option (similar to ESP-IDF example)
        int keepAlive = 1;
        int keepIdle = 10;    // seconds
        int keepInterval = 5; // seconds
        int keepCount = 3;    // count
        setsockopt(this->socketState, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(int));
        setsockopt(this->socketState, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(int));
        setsockopt(this->socketState, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(int));
        setsockopt(this->socketState, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(int));

        // Convert ip address to string

        if (source_addr.ss_family == PF_INET)
        {
            inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
        }
        else if (source_addr.ss_family == PF_INET6)
        {
            inet6_ntoa_r(((struct sockaddr_in6 *)&source_addr)->sin6_addr, addr_str, sizeof(addr_str) - 1);
        }
        else
        {
            ESP_LOGE("TCP SERVER", "Unknown address family: %d", source_addr.ss_family);
            break;
        }
        ESP_LOGW("TCP SERVER", "Socket accepted ip address: %s", addr_str);

        serverTask(instance->callback);
    }

    ESP_LOGE("TCP SERVER TASK", "Closing port: %d", instance->port);
    delete instance; // Liberar la memoria asignada dinámicamente
    this->cleanUpServer(listenSocket);
}

/**
 * Closes the specified socket and marks it as invalid.
 * Logs a message indicating the socket was closed.
 * @param listenSocket Reference to the socket descriptor to close.
 */
void TcpServer::cleanUpServer(int &listenSocket)
{
    close(listenSocket);
    listenSocket = -1;
    ESP_LOGE("TCP SERVER TASK", "Socket closed");
}

/**
 * Validates if the received buffer is a valid frame (starts with '{' and ends with '}').
 * If valid, null-terminates it and logs the frame.
 * @param buffer The received data buffer.
 * @param len The buffer length.
 * @return true if the frame is valid, false otherwise.
 */
bool TcpServer::isValidFrame(char *buffer, uint len)
{
    if (buffer[0] == '{')
    {
        for (size_t i = 0; i < len; i++)
        {
            if (buffer[i] == '}')
            {
                buffer[i + 1] = '\0'; // Null-terminate the string
                ESP_LOGI("Incoming frame validation", "Valid frame: %s", buffer);
                return true;
            }
        }
    }
    ESP_LOGE("Incoming frame validation", "%s", "Invalid frame");
    return false;
}

/**
 * Creates and launches a FreeRTOS task for the TCP server.
 * Copies the configuration to the heap to make it safe during the task's lifetime.
 * @param config The server configuration (port, callback, etc.).
 */
void TcpServer::createServer(IpServerConfiguration &config)
{
    // Copia la config al heap para que sea válida durante la tarea
    IpServerConfiguration *configCopy = new IpServerConfiguration(config);

    // Crea una estructura con el server y la config copiada
    TcpServer::TcpServerTaskArgs *args = new TcpServer::TcpServerTaskArgs{this, configCopy};

    xTaskCreate([](void *pvParameters)
                {
                    auto *args = static_cast<TcpServer::TcpServerTaskArgs *>(pvParameters);
                    args->server->serverLaunch(args->config);
                    delete args->config;
                    delete args;
                    vTaskDelete(NULL);
                },
                "TCP SERVER", TCP_TASK_SIZE, args, 5, NULL);
}