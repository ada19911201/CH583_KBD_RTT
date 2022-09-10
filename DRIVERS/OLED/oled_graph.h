#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "image_header.h"




void oled_clear_row(uint8_t row);
void oled_draw_text_16(uint8_t row, uint8_t col, const char* str);
void oled_draw_icon(uint8_t row, uint8_t col, const tImage* icon);

