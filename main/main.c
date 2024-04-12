#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "display_task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_mac.h"
#include "littlefs_util.h"
#include "softap_util.h"
#include "sta_util.h"
#include "param.h"
#include "button.h"
#include "http_client.h"
#include "cJSON.h"

static global_para_t *global_para = NULL;

void find_value_by_key(const char *str, const char *key, char *value)
{
    size_t i = 0;
    char *val_p = strstr(str, key) + strlen(key) + 1;
    for (i = 0; i < strlen(val_p); i++)
    {
        if (val_p[i] == '&' || val_p[i] == ' ')
        {
            if (i < BIND_INFO_LEN_MAX)
            {
                memcpy(value, val_p, i);
            }
            else
            {
                memcpy(value, val_p, BIND_INFO_LEN_MAX);
            }
            break;
        }
    }
}

void user_login(char *email, char *password)
{
    char request[512] = {0};
    char data[128] = {0};
    snprintf(data, sizeof(data), "email=%s&password=%s", email, password);
    snprintf(request, sizeof(request), POST_REQUEST_FONT, LOGIN_PATH, PLATFORM_HOST, PLATFORM_PORT, strlen(data), data);
    char *get_data = http_client_request(PLATFORM_HOST, PLATFORM_PORT, request);
    int offset = http_client_data_offset_get(get_data);
    cJSON *json = cJSON_Parse(get_data + offset);
    cJSON *code = cJSON_GetObjectItem(json, "code");
    if (code->valueint == 0)
    {
        cJSON *data = cJSON_GetObjectItem(json, "data");
        cJSON *token = cJSON_GetObjectItem(data, "token");
        strncpy(global_para->token, token->valuestring, strlen(token->valuestring) + 1);
    }
    cJSON_Delete(json);
    free(get_data);
}

void cloud_bind(char *name, char *position)
{
    char *request = (char *)malloc(1024);
    char data[512] = {0};
    snprintf(data, sizeof(data), "table=%s&name=%s&position=%s&token=%s", DATABASE, name, position, global_para->token);
    snprintf(request, 1024, POST_REQUEST_FONT, BIND_PATH, PLATFORM_HOST, PLATFORM_PORT, strlen(data), data);
    char *get_data = http_client_request(PLATFORM_HOST, PLATFORM_PORT, request);
    int offset = http_client_data_offset_get(get_data);
    cJSON *json = cJSON_Parse(get_data + offset);
    cJSON *code = cJSON_GetObjectItem(json, "code");
    if (code->valueint == 0)
    {
        cJSON *data = cJSON_GetObjectItem(json, "data");
        cJSON *did = cJSON_GetObjectItem(data, "did");
        global_para->did = did->valueint;
    }
    cJSON_Delete(json);
    free(request);
    free(get_data);
}

void app_main(void)
{
    global_para_init();
    global_para = global_para_get();

    // TaskHandle_t display_handle = NULL;
    // xTaskCreate(display_task_start, "display_task", 8192, NULL, 5, &display_handle);

    littlefs_util_init();

    button_init();

    struct stat st;
    if (stat(BIND_INFO_FILE, &st) != 0)
    {
        printf("no bind info\n");
        // 绑定文件不存在
        if (stat(CONNECT_INFO_FILE, &st) != 0)
        {
            // 联网文件不存在，起AP，等待用户提交联网绑定信息
            printf("no connect info\n");
            esp_efuse_mac_get_default(global_para->mac_addr);
            char ssid[32] = {0};
            snprintf(ssid, sizeof(ssid), "%s_%02x%02x%02x", PROJECT, global_para->mac_addr[0], global_para->mac_addr[1], global_para->mac_addr[2]);
            softap_util_startap(ssid, "");
        }
        else
        {
            printf("get connect info\n");
            FILE *f = fopen(CONNECT_INFO_FILE, "r");
            if (f == NULL) {
                printf("open connect info file err\n");
                littlefs_util_format();
                esp_restart();
            }
            char connect_info[1024] = {0};
            memset(connect_info, 0, sizeof(connect_info));
            size_t connect_info_len = fread(connect_info, 1, sizeof(connect_info), f);
            if (connect_info_len > 0)
            {
                connect_info[connect_info_len - 1] = ' ';
                // 联网
                int i = 0;
                char read_ssid[BIND_INFO_LEN_MAX] = {0};
                char read_psk[BIND_INFO_LEN_MAX] = {0};
                find_value_by_key(connect_info, "ssid", read_ssid);
                find_value_by_key(connect_info, "psk", read_psk);
                sta_util_connect(read_ssid, read_psk);
                for (i = 0; i < WAIT_REAL_IP_TIMES; i++)
                {
                    if (strlen(global_para->ip) > 7)
                    {
                        break;
                    }
                    sleep(1);
                }
                if (i >= WAIT_REAL_IP_TIMES - 1)
                {
                    printf("connect to network err\n");
                    littlefs_util_format();
                    esp_restart();
                }
                // 登录
                char email[BIND_INFO_LEN_MAX] = {0};
                char password[BIND_INFO_LEN_MAX] = {0};
                find_value_by_key(connect_info, "email", email);
                find_value_by_key(connect_info, "password", password);
                user_login(email, password);
                for (i = 0; i < WAIT_POST_TIMES; i++)
                {
                    if (strlen(global_para->token) > 0)
                    {
                        break;
                    }
                    sleep(1);
                }
                if (i >= WAIT_POST_TIMES - 1)
                {
                    printf("login err\n");
                    littlefs_util_format();
                    esp_restart();
                }
                // 绑定
                char name[BIND_INFO_LEN_MAX] = {0};
                char position[BIND_INFO_LEN_MAX] = {0};
                find_value_by_key(connect_info, "name", name);
                find_value_by_key(connect_info, "position", position);
                cloud_bind(name, position);
                for (i = 0; i < WAIT_POST_TIMES; i++)
                {
                    if (global_para->did > 0)
                    {
                        break;
                    }
                    sleep(1);
                }
                if (i >= WAIT_POST_TIMES - 1)
                {
                    printf("bind err\n");
                    littlefs_util_format();
                    esp_restart();
                }
                printf("did:%u\n", global_para->did);
            }
            else
            {
                littlefs_util_format();
                esp_restart();
            }
        }
    }
    else
    {
        printf("get bind info\n");

    }

    while (1)
    {
        sleep(1);
        printf("%s\n", "alive");
    }

    littlefs_util_deinit();
}
