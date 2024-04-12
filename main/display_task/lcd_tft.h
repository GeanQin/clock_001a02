#ifndef _LCD_TFT_H_
#define _LCD_TFT_H_

#include "esp_lcd_panel_io.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define PIN_LCD_SPI_SCK 6   // SPI时钟线
#define PIN_LCD_SPI_SDA 7   // SPI数据线(MOSI)，主设备输出，从设备输入，单向传输
#define PIN_LCD_SPI_CS 0    // SPI片选，用于Master选Slave，比如低电平选中
#define PIN_LCD_DC 19       // 数据命令选择，比如低电平写命令，高电平写数据
#define PIN_LCD_RST 5       // 复位，比如低复位，高运行
#define PIN_LCD_BK_LIGHT 18 // 屏幕背光
#define LCD_BK_LIGHT_ON_LEVEL 1
#define LCD_BK_LIGHT_OFF_LEVEL !LCD_BK_LIGHT_ON_LEVEL
#define LCD_SCK_HZ (20 * 1000 * 1000)
#define LCD_H 240
#define LCD_V 240
#define LCD_CMD_BITS 8
#define LCD_PARAM_BITS 8

esp_lcd_panel_handle_t lcd_init();

#ifdef __cplusplus
}
#endif

#endif