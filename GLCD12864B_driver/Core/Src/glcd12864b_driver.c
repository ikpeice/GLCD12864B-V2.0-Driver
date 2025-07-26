/*
 * glcd12863b_driver.c
 *
 *  Created on: Jul 20, 2025
 *      Author: echij
 */


#include "glcd12864b_driver.h"
#include "delay.h"
#include "fonts.h"
#include <string.h>



uint8_t framebuffer[64][16]={0};  // 64 rows, 128/8 = 16 columns

// Each character is 5 columns wide and 7 rows tall
// Stored as 5 bytes per character (one byte per column, bottom-up)






void LCD_EnablePulse() {
    HAL_GPIO_WritePin(LCD_E_GPIO, LCD_E_PIN, GPIO_PIN_SET);
    delay_us(500);
    HAL_GPIO_WritePin(LCD_E_GPIO, LCD_E_PIN, GPIO_PIN_RESET);
    delay_us(40);
}

void LCD_WriteByte(uint8_t data) {
	HAL_GPIO_WritePin(LCD_RW_GPIO, LCD_RW_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_E_GPIO, LCD_E_PIN, GPIO_PIN_SET);
    delay_us(500);
    LCD_DATA_GPIO->ODR = (LCD_DATA_GPIO->ODR & 0xFF00) | data;
    HAL_GPIO_WritePin(LCD_E_GPIO, LCD_E_PIN, GPIO_PIN_RESET);
    delay_us(40);
//    LCD_EnablePulse();
}

void LCD_WriteCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(LCD_RS_GPIO, LCD_RS_PIN, GPIO_PIN_RESET);
    LCD_WriteByte(cmd);
}

void LCD_WriteData(uint8_t data) {
    HAL_GPIO_WritePin(LCD_RS_GPIO, LCD_RS_PIN, GPIO_PIN_SET);
    LCD_WriteByte(data);
}



void lcd_reset(){
    HAL_GPIO_WritePin(LCD_RST_GPIO, LCD_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LCD_RST_GPIO, LCD_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(100);
    memset(framebuffer,0,sizeof(framebuffer));
}

void LCD_Init(void) {

	lcd_reset();
    LCD_WriteCommand(0x30);
    HAL_Delay(1);
    LCD_WriteCommand(0x30);
    HAL_Delay(1);
    LCD_WriteCommand(0x0C);
    HAL_Delay(1);
    LCD_WriteCommand(0x01);
    HAL_Delay(2);

    LCD_WriteCommand(0x34); // Extended mode: graphics ON
    HAL_Delay(1);
    LCD_WriteCommand(0x36); // Extended mode: graphics ON
    HAL_Delay(1);
    LCD_WriteCommand(0x12); // Extended mode: graphics ON
    HAL_Delay(1);
//    LCD_WriteCommand(0x01); // clear
//    HAL_Delay(1);
}

void lcd_shiftleft(){
	LCD_WriteCommand(0x07);
	HAL_Delay(10);
}

void LCD_DrawBuffer() {
    for (uint8_t y = 0; y < 32; y++) {
        uint8_t x;
        LCD_WriteCommand(0x80 | y);
        LCD_WriteCommand(0x80);
        for (x = 0; x < 16; x++) {
            //uint8_t left = framebuffer[y * 16 + x];
            //LCD_WriteData(left);
        }
    }
}

void setPixel(uint8_t x, uint8_t y, uint8_t color) {
    if (x >= 64 || y >= 128) return;
    uint8_t page = y / 8;
    uint8_t bit  = 7 - (y % 8);  // top row is bit 7
    if (color)
        framebuffer[x][page] |=  (1 << bit);
    else
        framebuffer[x][page] &= ~(1 << bit);
}




void drawTestPattern() {
    memset(framebuffer, 0x00, sizeof(framebuffer));

//    for (int x = 0; x < 32; x++) {
//        int y1 = (x * 32) / 32;         // Diagonal from top-left to bottom-right
//        int y2 = 31 - ((x * 32) / 32);  // Diagonal from bottom-left to top-right
//        setPixel(x, y1, 1);
//        setPixel(x, y2, 1);
//    }

    for (int x = 0; x < 128; x++) {
        int y1 = (x * 64) / 128;
        int y2 = 63 - y1;
        setPixel(x, y1, 1);
        setPixel(x, y2, 1);
    }
    LCD_DrawBuffer();
}

void drawChar(uint8_t x, uint8_t y, char c) {
    if (c < 32 || c > 126) return;
    const uint8_t *bitmap = font5x7[c - 32];

    for (uint8_t col = 0; col < 5; col++) {
        uint8_t line = bitmap[col];
        for (uint8_t row = 0; row < 7; row++) {
            if (line & (1 << row)) {
                setPixel(x + col, y + row, 1);
            }
        }
    }
}


void drawString(uint8_t x, uint8_t y, const char *str) {
    while (*str) {
        drawChar(x, y, *str++);
        x += 6;  // 5 pixels wide + 1 space
        if (x > 122) break;  // Prevent wrap
    }
}

void test_pixel(){
	for (int x = 0; x < 128; x++) {
	    setPixel(x, 40, 1);  // Middle row, should span full width
	}
}



void drawXPattern2() {
    for (int i = 0; i < 64; i++) {
        setPixel(i, i, 1);               // top-left to bottom-right
        setPixel(127 - i, i, 1);         // top-right to bottom-left
    }
}


void drawFullGrid() {
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 128; x += 4) {
            setPixel(x, y, 1);  // Draw every 4th pixel
        }
    }
}

void lcd_clear(){
	memset(framebuffer,0,sizeof(framebuffer));
	st7920_update();
}



