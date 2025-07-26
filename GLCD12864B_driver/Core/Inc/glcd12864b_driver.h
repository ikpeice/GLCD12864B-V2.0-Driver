/*
 * glcd12864b_driver.h
 *
 *  Created on: Jul 20, 2025
 *      Author: echij
 */

#ifndef INC_GLCD12864B_DRIVER_H_
#define INC_GLCD12864B_DRIVER_H_
#include "main.h"

#include "main.h"  // Replace with your STM32 series header

#define ST7920_WIDTH 128
#define ST7920_HEIGHT 64

#define LCD_RS_GPIO GPIOA
#define LCD_RS_PIN  GPIO_PIN_0
#define LCD_E_GPIO  GPIOA
#define LCD_E_PIN   GPIO_PIN_1
#define LCD_RST_GPIO GPIOA
#define LCD_RST_PIN  GPIO_PIN_10
#define LCD_RW_PIN	GPIO_PIN_8
#define LCD_RW_GPIO GPIOA
#define LCD_DATA_GPIO GPIOB

void LCD_Init(void);
void drawTestPattern();
void LCD_DrawBuffer();
void drawString(uint8_t x, uint8_t y, const char *str);
void lcd_clear();
void lcd_shiftleft();
void drawXPattern2();
void drawFullGrid();
void st7920_update(void);

void setPixel(uint8_t x, uint8_t y, uint8_t color);

void lcd_reset();
void drawXPattern(void);

void test_pixel();
#endif /* INC_GLCD12864B_DRIVER_H_ */
