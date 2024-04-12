#include <string.h>
#include "freertos/FreeRTOS.h"
#include "lvgl.h"
#include "esp_heap_caps.h"
#include "esp_timer.h"
#include "lvgl_display.h"

static void lvgl_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *color_map)
{
    esp_lcd_panel_handle_t panel_handle = lv_display_get_user_data(disp);
    int len = (area->x2 + 1 - area->x1) * (area->y2 + 1 - area->y1);
    lv_draw_sw_rgb565_swap(color_map, len);
    esp_lcd_panel_draw_bitmap(panel_handle, area->x1, area->y1, area->x2 + 1, area->y2 + 1, color_map);
    lv_display_flush_ready(disp);
}

static void increase_lvgl_tick(void *arg)
{
    lv_tick_inc(2);
}

void lvgl_display_page(page_e page, const page_txt_t *txt, const lv_image_dsc_t **imgs, int img_count)
{
    lv_obj_t *screen = lv_scr_act();
    lv_obj_set_style_bg_color(screen, lv_color_make(0, 0, 0), 0);
    if (page < PAGE_MAIN)
    {
        if (page == PAGE_ALARM)
        {
            lv_obj_t *tip = lv_obj_create(screen);
            static lv_style_t tip_style;
            lv_style_init(&tip_style);
            lv_style_set_bg_color(&tip_style, lv_color_make(98, 98, 98));
            lv_style_set_text_color(&tip_style, lv_color_make(255, 255, 255));
            lv_style_set_text_font(&tip_style, &lv_font_montserrat_14);
            lv_style_set_size(&tip_style, strlen(txt->tip) * 9, 48);
            lv_style_set_border_width(&tip_style, 0);
            lv_style_set_radius(&tip_style, 22);
            lv_obj_add_style(tip, &tip_style, 0);
            lv_obj_align(tip, LV_ALIGN_CENTER, 0, -48);

            lv_obj_t *tip_label= lv_label_create(tip);
            lv_label_set_text(tip_label, txt->tip);

            lv_obj_t * bottom_icon = lv_animimg_create(screen);
            lv_obj_align(bottom_icon, LV_ALIGN_BOTTOM_MID, 0, -16);
            lv_animimg_set_src(bottom_icon, (const void **)imgs, img_count);
            lv_animimg_set_duration(bottom_icon, 2000);
            lv_animimg_set_repeat_count(bottom_icon, LV_ANIM_REPEAT_INFINITE);
            lv_animimg_start(bottom_icon);
        }
        else if (page == PAGE_FAIL)
        {
            lv_obj_t *fail_icon = lv_image_create(screen);
            LV_IMAGE_DECLARE(fail_img);
            lv_image_set_src(fail_icon, &fail_img);
            lv_obj_align(fail_icon, LV_ALIGN_CENTER, 0, -48);
        }
        else if (page == PAGE_SUCCESS)
        {
            lv_obj_t *success_icon = lv_image_create(screen);
            LV_IMAGE_DECLARE(success_img);
            lv_image_set_src(success_icon, &success_img);
            lv_obj_align(success_icon, LV_ALIGN_CENTER, 0, -48);
        }

        lv_obj_t *content = lv_obj_create(screen);
        static lv_style_t content_style;
        lv_style_init(&content_style);
        lv_style_set_bg_color(&content_style, lv_color_make(0, 0, 0));
        lv_style_set_text_color(&content_style, lv_color_make(255, 255, 255));
        lv_style_set_text_font(&content_style, &lv_font_montserrat_12);
        lv_style_set_size(&content_style, 192, 100);
        lv_style_set_border_width(&content_style, 0);
        lv_obj_add_style(content, &content_style, 0);
        lv_obj_align(content, LV_ALIGN_CENTER, 0, 24);

        for (int i = 0; i < sizeof(txt->connent) / sizeof(txt->connent[0]); i++)
        {
            lv_obj_t *content_label = lv_label_create(content);
            lv_obj_align(content_label, LV_ALIGN_CENTER, 0, (i - 1) * 16);
            lv_label_set_text(content_label, txt->connent[i]);
        }
    }
}

void lvgl_display_start(esp_lcd_panel_handle_t panel_handle)
{
    lv_init();
    uint16_t *pdata1 = heap_caps_malloc(BUF_LEN * 2, MALLOC_CAP_DMA);
    uint16_t *pdata = heap_caps_malloc(BUF_LEN * 2, MALLOC_CAP_DMA);
    lv_display_t *ui_disp = lv_display_create(240, 240);
    lv_display_set_flush_cb(ui_disp, lvgl_flush_cb);
    lv_display_set_user_data(ui_disp, panel_handle);
    lv_display_set_buffers(ui_disp, pdata, pdata1, BUF_LEN * 2, LV_DISPLAY_RENDER_MODE_PARTIAL);

    const esp_timer_create_args_t lvgl_tick_timer_args = {.callback = &increase_lvgl_tick, .name = "lvgl_tick"};
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, 2 * 1000));

    lv_obj_t *screen = lv_scr_act();
    lv_obj_set_style_bg_color(screen, lv_color_make(0, 0, 0), 0);

    while (1)
    {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
