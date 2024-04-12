#ifndef _STA_UTIL_H_
#define _STA_UTIL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define CONNECT_RETRY_TIMES 64

void sta_util_connect(char *ssid, char *psk);

#ifdef __cplusplus
}
#endif

#endif

