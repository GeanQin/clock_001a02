#ifndef _PARAM_H_
#define _PARAM_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define PROJECT "clock_001a02"
#define DATABASE "clock_001a02_1"
#define BIND_INFO_LEN_MAX 64
#define WAIT_REAL_IP_TIMES 64
#define WAIT_POST_TIMES 8
#define CONNECT_INFO_FILE "/littlefs/connect.conf"
#define BIND_INFO_FILE "/littlefs/bind.conf"

typedef enum {
    weather_none = 0,
    weather_sun = 1,
    weather_cloudy,
    weather_wind,
    weather_haze,
    weather_rain,
    weather_snow,
    weather_fog
} weather_code_e;

typedef struct
{
    bool is_bind;
    unsigned int did;
    unsigned char mac_addr[6];
    char ip[16];
    char token[192];
    weather_code_e weather_code;
    float humidity;
    float temperature;
} global_para_t;

void global_para_init();
global_para_t *global_para_get();

#ifdef __cplusplus
}
#endif

#endif
