#ifndef _HTTP_HPP_
#define _HTTP_HPP_

#include <GlobalDefines.hpp>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_tls.h>
#include <esp_http_client.h>

#include "ProtocolsInterface.hpp"

#define MAX_HTTP_RECV_BUFFER 512
#define MAX_HTTP_OUTPUT_BUFFER 2048

typedef struct _ApiConfig
{
    string host;
} ApiConfig;

class Http 
{
private:
    static constexpr string tag = "HTTP";
    static void get(void *pvParameters);
    static esp_err_t webApiEventHandler(esp_http_client_event_t *event);

public:
    Http();
    static bool httpGet(string url);
    ~Http();
};

#endif