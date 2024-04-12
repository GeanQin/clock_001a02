#ifndef _HTTP_CLIENT_H_
#define _HTTP_CLIENT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define RECV_LEN_MAX 1024
#define IP_HOST "api.ipify.org"
#define IP_PORT "80"
#define IP_PATH "/"
#define PLATFORM_HOST "www.geanchin.cn"
#define PLATFORM_PORT "8000"
#define LOGIN_PATH "/user/login"
#define BIND_PATH "/device/bind"
#define UNBIND_PATH "/device/unbind"
#define UPLOAD_URL "/device/update_info"
#define WEATHER_PATH "/weather_get"
#define GET_REQUEST_FONT "GET %s HTTP/1.1\r\n" \
                         "Host: %s:%s\r\n" \
                         "User-Agent: esp-idf/1.0 esp32\r\n" \
                         "\r\n"
#define POST_REQUEST_FONT "POST %s HTTP/1.1\r\n" \
                          "Host: %s:%s\r\n" \
                          "User-Agent: esp-idf/1.0 esp32\r\n" \
                          "Content-Type: application/x-www-form-urlencoded\r\n" \
                          "Content-Length: %d\r\n" \
                          "Connection: Keep-Alive\r\n" \
                          "\r\n" \
                          "%s\r\n"

char *http_client_request(const char *host, const char *port, char *request);
int http_client_data_offset_get(char *data);

#ifdef __cplusplus
}
#endif

#endif

