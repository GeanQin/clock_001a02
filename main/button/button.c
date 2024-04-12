#include "driver/gpio.h"
#include "esp_attr.h"
#include "button.h"
#include "esp_system.h"

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    switch (gpio_num)
    {
    case RESET_BTN_PIN:
        esp_restart();
        break;
    
    default:
        break;
    }
}

void button_init()
{
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(RESET_BTN_PIN, gpio_isr_handler, (void *)RESET_BTN_PIN);
    gpio_isr_handler_add(FLIP_BTN_PIN, gpio_isr_handler, (void *)FLIP_BTN_PIN);
    gpio_isr_handler_add(OK_BTN_PIN, gpio_isr_handler, (void *)OK_BTN_PIN);
}