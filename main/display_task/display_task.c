#include <string.h>
#include "esp_lcd_panel_ops.h"
#include "display_task.h"
#include "lcd_tft.h"

const page_txt_t page_bind_txt = {"Please connect to WiFi", {"WIFI: myssid", "Open 'http://192.168.4.1'", "by browser"}};
LV_IMAGE_DECLARE(wifi1_img);
LV_IMAGE_DECLARE(wifi2_img);
LV_IMAGE_DECLARE(wifi3_img);
LV_IMAGE_DECLARE(wifi4_img);
static const lv_image_dsc_t * wifi_imgs[] = {
    &wifi1_img,
    &wifi2_img,
    &wifi3_img,
    &wifi4_img,
    &wifi1_img,
};

void display_task_start(void *param)
{
    esp_lcd_panel_handle_t panel_handle = lcd_init();
    lvgl_display_start(panel_handle);
}

void display_task_show_success(page_txt_t *txt)
{
    lvgl_display_page(PAGE_SUCCESS, txt, NULL, 0);
}

void display_task_show_fail(page_txt_t *txt)
{
    lvgl_display_page(PAGE_FAIL, txt, NULL, 0);
}

void display_task_show_bind()
{
    lvgl_display_page(PAGE_ALARM, &page_bind_txt, wifi_imgs, sizeof(wifi_imgs) / sizeof(lv_image_dsc_t *));
}