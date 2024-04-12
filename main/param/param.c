#include <string.h>
#include "param.h"

static global_para_t global_para;

void global_para_init()
{
    global_para.is_bind = false;
    global_para.did = 0;
    memset(global_para.mac_addr, 0, sizeof(global_para.mac_addr));
    memset(global_para.ip, 0, sizeof(global_para.ip));
    memset(global_para.token, 0, sizeof(global_para.token));
    global_para.weather_code = weather_none;
    global_para.humidity = 50.0;
    global_para.temperature = 15;
}

global_para_t *global_para_get()
{
    return &global_para;
}