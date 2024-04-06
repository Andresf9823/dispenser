#ifndef _HTTP_HPP_
#define _HTTP_HPP_

#include <GlobalDefines.hpp>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_http_client.h>

using namespace std;

#define MAX_HTTP_RECV_BUFFER 512
#define MAX_HTTP_OUTPUT_BUFFER 2048

typedef struct _ApiConfig
{
    string host;
} ApiConfig;

class Http
{
private:
    static constexpr string tag = "API CLIENT";
    static string path;
    static void _Get(void *pvParameters);
    static esp_err_t WebApiEventHandler(esp_http_client_event_t *event);
    string target;

public:
    Http();
    void setPath(string path);
    ApiConfig GetConfig();
    void Get(string path);
    ~Http();
};

#endif