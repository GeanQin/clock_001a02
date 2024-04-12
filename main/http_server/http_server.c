
#include "http_server.h"
#include "esp_check.h"
#include "esp_system.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "param.h"

static const char *TAG = "bind";

static esp_err_t bind_page_get_handler(httpd_req_t *req)
{
    const char *resp_str = (const char *)req->user_ctx;
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t bind_info_get_handler(httpd_req_t *req)
{
    int get_info_flag = 0;
    char *buf;
    size_t buf_len;

    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1)
    {
        buf = malloc(buf_len);
        ESP_RETURN_ON_FALSE(buf, ESP_ERR_NO_MEM, TAG, "buffer alloc failed");
        /* Copy null terminated value string into buffer */
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK)
        {
            ESP_LOGI(TAG, "Found header => Host: [%s]", buf);
        }
        free(buf);
    }

    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1)
    {
        buf = malloc(buf_len);
        ESP_RETURN_ON_FALSE(buf, ESP_ERR_NO_MEM, TAG, "buffer alloc failed");
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK)
        {
            get_info_flag = 1;

            ESP_LOGI(TAG, "Opening connect info file");
            FILE *f = fopen(CONNECT_INFO_FILE, "w");
            if (f == NULL)
            {
                ESP_LOGE(TAG, "Failed to open connect info file for writing");
            }
            else
            {
                fwrite(buf, 1, buf_len, f);
                fclose(f);
                ESP_LOGI(TAG, "File connect info written");
            }
        }
        free(buf);
    }

    if (get_info_flag)
    {
        const char *resp_str = (const char *)req->user_ctx;
        httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
    }
    else
    {
        httpd_resp_send(req, BIND_WEB, HTTPD_RESP_USE_STRLEN);
    }

    esp_restart();

    return ESP_OK;
}

static const httpd_uri_t bind_page = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = bind_page_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx = BIND_WEB};

static const httpd_uri_t bind_info = {
    .uri = "/bind",
    .method = HTTP_GET,
    .handler = bind_info_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx = "<h1>wait connectting...</h1>"};

httpd_handle_t http_server_start()
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK)
    {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &bind_page);
        httpd_register_uri_handler(server, &bind_info);
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}