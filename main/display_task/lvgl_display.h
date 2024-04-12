#ifndef _LVGL_DISPLAY_H_
#define _LVGL_DISPLAY_H_

#include "esp_lcd_panel_ops.h"
#include "lvgl.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define BUF_LEN (240 * 12)

typedef enum
{
    PAGE_ALARM = 1,
    PAGE_SUCCESS,
    PAGE_FAIL,
    PAGE_MAIN

} page_e;

typedef struct
{
    char tip[28];
    char connent[3][32];
} page_txt_t;

void lvgl_display_start(esp_lcd_panel_handle_t panel_handle);
void lvgl_display_page(page_e page, const page_txt_t *txt, const lv_image_dsc_t **imgs, int img_count);


#ifdef __cplusplus
}
#endif

#endif