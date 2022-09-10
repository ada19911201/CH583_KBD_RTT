/*
Copyright (C) 2020 Jim Jiang <jim@lotlab.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <stdlib.h>
#include <string.h>

#include "oled_graph.h"
#include "ssd1306_oled.h"

#include "font_5x8.h"
#include "font_gohu.h"


/**
 * @brief 清空指定行的缓存
 * 
 * @param row 
 */
void oled_clear_row(uint8_t row)
{
    memset(&ssd1306_display_buffer[row * SSD1306_COLS], 0, SSD1306_COLS);
    // ssd1306_buff_dirty[row] = true;
}

/**
 * @brief 在指定行显示16x8的字符
 * 
 * @param row 行
 * @param align 对齐方式 
 * @param offset 偏移
 * @param str 字符串
 */
void oled_draw_text_16(uint8_t row, uint8_t col, const char* str)
{
    oled_clear_row(row);
    oled_clear_row(row + 1);

    // 计算偏移
    uint16_t buff_offset = row * SSD1306_COLS + col;

    while (*str) {
        uint8_t* pointer = (uint8_t*)GohuFont.chars[0].image->data;
        if (*str >= 0x20 && *str <= 0x7E) {
            pointer = (uint8_t*)GohuFont.chars[(*str - 0x20)].image->data;
        }
        for (uint8_t i = 0; i < 8; i++) {
            ssd1306_display_buffer[buff_offset + i] = pointer[i];
            ssd1306_display_buffer[buff_offset + i + SSD1306_COLS] = pointer[i + 8];
        }
        buff_offset += 8;
        str++;
    }
}

/**
 * @brief 在指定位置上画一个图标
 * 
 * @param row 行
 * @param col 列
 * @param icon 图标
 */
void oled_draw_icon(uint8_t row, uint8_t col, const tImage* icon)
{
    uint8_t bit = (icon->height - 1) / 8 + 1;
    for (uint8_t c = col, i = 0; c < SSD1306_COLS && i < icon->width; i++, c++) {
        for (uint8_t r = row, j = 0; r < SSD1306_ROWS && j < bit; j++, r++) {
            ssd1306_display_buffer[r * SSD1306_COLS + c] = icon->data[j * icon->width + i];
        }
    }
}

