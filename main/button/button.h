#ifndef _BUTTON_H_
#define _BUTTON_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define ESP_INTR_FLAG_DEFAULT 0
#define RESET_BTN_PIN 9 // 短按重启，长按重置
#define FLIP_BTN_PIN 10 // 翻页
#define OK_BTN_PIN 3 // 确认
#define GPIO_INPUT_PIN_SEL ((1ULL << RESET_BTN_PIN) | (1ULL << FLIP_BTN_PIN) | (1ULL << OK_BTN_PIN))

void button_init();

#ifdef __cplusplus
}
#endif

#endif