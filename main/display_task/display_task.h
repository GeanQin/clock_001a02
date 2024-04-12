#ifndef _DISPLAY_TASK_H_
#define _DISPLAY_TASK_H_

#include "lvgl_display.h"

#ifdef __cplusplus
extern "C"
{
#endif

void display_task_start(void *param);
void display_task_show_success(page_txt_t *txt);
void display_task_show_fail(page_txt_t *txt);
void display_task_show_bind();

#ifdef __cplusplus
}
#endif

#endif