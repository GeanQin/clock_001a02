#ifndef _LITTLEFS_UTIL_H_
#define _LITTLEFS_UTIL_H_

#ifdef __cplusplus
extern "C"
{
#endif

int littlefs_util_init();
void littlefs_util_format();
void littlefs_util_deinit();

#ifdef __cplusplus
}
#endif

#endif