#include <WebApiConsumer.hpp>

WebApiConsumer::WebApiConsumer(string target)
{
    this->target = target;
}

esp_err_t WebApiConsumer::WebApiEventHandler(esp_http_client_event_t *event)
{
    static char *bufferOut;
    static int bufferOutLen = 0;
    switch (event->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGE(tag.c_str(), "HTTP_EVENT_ERROR"); // This event occurs when there are any errors during execution
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGI(tag.c_str(), "HTTP_EVENT_ON_CONNECTED"); // Once the HTTP has been connected to the server, no data exchange has been performed
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGI(tag.c_str(), "HTTP_EVENT_ON_HEADER"); // Occurs when receiving each header sent from the server
        ESP_LOGI(tag.c_str(), "HEADER: {%s: %s}", event->header_key, event->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGI(tag.c_str(), "HTTP_EVENT_ON_DATA"); // Occurs when receiving data from the server, possibly multiple portions of the packet
        memset(bufferOut, 0, MAX_HTTP_OUTPUT_BUFFER);

        if (!esp_http_client_is_chunked_response(event->client))
        {
            // If user_data buffer is configured, copy the response into the buffer
            int copy_len = 0;
            if (event->user_data)
            {
                // The last byte in event->user_data is kept for the NULL character in case of out-of-bound access.
                copy_len = abs(event->data_len - bufferOutLen);
                if (copy_len)
                {
                    memcpy(event->user_data, event->data, copy_len);
                }
            }
            else
            {
                int content_len = esp_http_client_get_content_length(event->client);
                if (bufferOut == NULL)
                {
                    //     // We initialize bufferOut with 0 because it is used by strlen() and similar functions therefore should be null terminated.
                    bufferOut = (char *)calloc(content_len + 1, sizeof(char));
                    bufferOutLen = 0;
                    if (bufferOut == NULL)
                    {
                        ESP_LOGI(tag.c_str(), "Failed to allocate memory for output buffer");
                    }
                }
                copy_len = abs(event->data_len - bufferOutLen);
                if (copy_len)
                {
                    memcpy(bufferOut + bufferOutLen, event->data, copy_len);
                }
            }
            bufferOutLen += copy_len;
        }
        break;
    case HTTP_EVENT_ON_FINISH: // Occurs when finish a HTTP session
        ESP_LOGI(tag.c_str(), "HTTP_EVENT_ON_FINISH");
        if (bufferOut != NULL)
        {
            free(bufferOut);
        }
        bufferOut = 0;
        break;
    case HTTP_EVENT_DISCONNECTED: // The connection has been disconnected
        ESP_LOGE(tag.c_str(), "HTTP_EVENT_DISCONNECTED");
        bufferOut = 0;
        break;
    case HTTP_EVENT_REDIRECT: // Intercepting HTTP redirects to handle them manually
        ESP_LOGI(tag.c_str(), "HTTP_EVENT_ON_FINISH");
        break;

    default:
        break;
    }
    return ESP_OK;

}

ApiConfig WebApiConsumer::GetConfig()
{
    ApiConfig config;
    config.host = this->target;
    return config;
}

void WebApiConsumer::Get(string path)
{
    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER + 1] = {0};
    esp_http_client_config_t config = {
        .host = this->target.c_str(),
        .path = path.c_str(),
        .query = "",
        .disable_auto_redirect = true,
        .event_handler = WebApiEventHandler,
        .user_data = local_response_buffer,
    };
    xTaskCreate(&_Get, "GET", 4096, &config, 5, NULL);
}

void WebApiConsumer::_Get(void *pvParameters)
{
    esp_http_client_handle_t client = esp_http_client_init((esp_http_client_config_t *)pvParameters);
    esp_http_client_set_method(client, HTTP_METHOD_GET);

    if (esp_http_client_perform(client) == ESP_OK)
    {
        ESP_LOGI(tag.c_str(), "HTTP GET STATUS %d, content length %" PRId64, esp_http_client_get_status_code(client), esp_http_client_get_content_length(client));
    }
    else
    {
        ESP_LOGE(tag.c_str(), "HTTP GET REQUEST FAILED");
    }
    vTaskDelete(NULL);
}

WebApiConsumer::~WebApiConsumer()
{
}