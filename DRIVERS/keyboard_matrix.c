/*
Copyright (C) 2018,2019 Jim Jiang <jim@lotlab.org>

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
/**
 * @brief 键盘按键扫描
 * 
 */
#include <stdbool.h>
#include <stdint.h>
#include "matrix.h"
#include "config.h"
#include "xprintf.h"
#include "util.h"
#include "wait_api.h"
#include "CH58x_common.h"
#ifdef RGB_TOP_ENABLE
    #include "rgb_matrix.h"
#endif

#ifndef DEBOUNCE
#define DEBOUNCE 1
#endif


static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void select_row(uint8_t row);
static void unselect_rows(uint8_t row);

/**
 * @brief 初始化键盘阵列
 * 
 */
void matrix_init(void)
{
    for (uint_fast8_t i = MATRIX_ROWS; i--;) {
        if((uint32_t)MATRIX_ROW_PINS[i]<16){
            GPIOA_ModeCfg(1<<MATRIX_ROW_PINS[i],GPIO_ModeOut_PP_5mA);
        }else{
            GPIOB_ModeCfg(1<<(MATRIX_ROW_PINS[i]-16),GPIO_ModeOut_PP_5mA);
        }
    }
    for (uint_fast8_t i = MATRIX_COLS; i--;) {
        if((uint32_t)MATRIX_COL_PINS[i]<16){
            GPIOA_ModeCfg(1<<MATRIX_COL_PINS[i],GPIO_ModeIN_PD);
        }else{
            GPIOB_ModeCfg(1<<(MATRIX_COL_PINS[i]-16),GPIO_ModeIN_PD);
        }
    }
}
/**
* @brief 释放键盘阵列
 * 
 */
void matrix_uninit(void)
{
    for (uint_fast8_t i = MATRIX_ROWS; i--;) {
        if((uint32_t)MATRIX_ROW_PINS[i]<16){
            GPIOA_ModeCfg(1<<MATRIX_ROW_PINS[i],GPIO_ModeIN_Floating);
        }else{
            GPIOB_ModeCfg(1<<(MATRIX_ROW_PINS[i]-16),GPIO_ModeIN_Floating);
        }
    }
    for (uint_fast8_t i = MATRIX_COLS; i--;) {
        if((uint32_t)MATRIX_COL_PINS[i]<16){
            GPIOA_ModeCfg(1<<MATRIX_COL_PINS[i],GPIO_ModeIN_Floating);
        }else{
            GPIOB_ModeCfg(1<<(MATRIX_COL_PINS[i]-16),GPIO_ModeIN_Floating);
        }
    }
}

static bool read_col(uint8_t col){
    if(col<16){
        return GPIOA_ReadPortPin(1<<col);
    }else{
        return GPIOB_ReadPortPin(1<<(col-16));
    }
}
/** read all rows */
static matrix_row_t read_cols(void)
{
    matrix_row_t result = 0;

    for (uint_fast8_t c = 0; c < MATRIX_COLS; c++) {
        if (read_col((uint8_t)MATRIX_COL_PINS[c]))
            result |= 1UL << c; 
    }
    return result;
}

static void select_row(uint8_t row)
{
    if(row<16){
        GPIOA_SetBits(1<<row);
    }else{
        GPIOB_SetBits(1<<(row-16));
    }
}

static void unselect_rows(uint8_t row)
{
    if(row<16){
        GPIOA_ResetBits(1<<row);
    }else{
        GPIOB_ResetBits(1<<(row-16));
    }
}


//键盘矩阵扫描程序
static void matrix_scan_kb(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
#ifdef HYBRID_MATRIX
        init_cols();
#endif
        // delay_us(1);
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            debouncing = DEBOUNCE;
        }
        unselect_rows(i);
    }
    if (debouncing) {
        if (--debouncing) {
            // no need to delay here manually, because we use the clock.
            wait_ms(1);
            // keyboard_debounce();
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }
}
//zoro键盘扫描核心
static void matrix_scan_zoro(void){
    matrix_scan_kb();
}
uint8_t matrix_scan(void) {
    matrix_scan_zoro();
    return 1;
}

bool matrix_is_modified(void)
{
    if (debouncing)
        return false;
    return true;
}

inline matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop16(matrix[i]);
    }
    return count;
}
/**
 * @brief 阵列准备睡眠
 * 
 */
void matrix_wakeup_prepare(void)
{

}