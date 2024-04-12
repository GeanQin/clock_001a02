#include "esp_http_client.h"
#include "esp_tls.h"
#include "esp_event.h"
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "http_client.h"

static const char *TAG = "http_client";

char *http_client_request(const char *host, const char *port, char *request)
{
    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;
    int s, r;
    char *recv_buf = (char *)malloc(RECV_LEN_MAX);

    int err = getaddrinfo(host, port, &hints, &res);
    if (err != 0 || res == NULL)
    {
        ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        return NULL;
    }

    s = socket(res->ai_family, res->ai_socktype, 0);
    if (s < 0)
    {
        ESP_LOGE(TAG, "... Failed to allocate socket.");
        freeaddrinfo(res);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        return NULL;
    }
    if (connect(s, res->ai_addr, res->ai_addrlen) != 0)
    {
        ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
        close(s);
        freeaddrinfo(res);
        vTaskDelay(4000 / portTICK_PERIOD_MS);
        return NULL;
    }
    freeaddrinfo(res);

    if (write(s, request, strlen(request)) < 0)
    {
        ESP_LOGE(TAG, "... socket send failed");
        close(s);
        vTaskDelay(4000 / portTICK_PERIOD_MS);
        return NULL;
    }

    struct timeval receiving_timeout;
    receiving_timeout.tv_sec = 5;
    receiving_timeout.tv_usec = 0;
    if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout,
                   sizeof(receiving_timeout)) < 0)
    {
        ESP_LOGE(TAG, "... failed to set socket receiving timeout");
        close(s);
        vTaskDelay(4000 / portTICK_PERIOD_MS);
        return NULL;
    }

    bzero(recv_buf, RECV_LEN_MAX);
    r = read(s, recv_buf, RECV_LEN_MAX - 1);
    close(s);
    if (r > 0)
    {
        return recv_buf;
    }
    else
    {
        free(recv_buf);
        ESP_LOGE(TAG, "read from socket err.");
        return NULL;
    }
}

int http_client_data_offset_get(char *data)
{
    for (int i = 3; i < strlen(data); i++)
    {
        if (data[i] == '\n' && data[i - 1] == '\r' && data[i - 2] == '\n' && data[i - 3] == '\r')
        {
            return i + 1;
        }
    }
    return -1;
}