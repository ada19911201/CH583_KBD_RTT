#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "config.h"



#define MAX_LAYER 8 // 存储区域中最大层数
#define MAX_FN_KEYS 32 // 存储区域最大Fn数目
#define MAX_MACRO_SIZE 1024 // 宏存储区域最大容量

#define SINGLE_KEY_SIZE 2
#define KEYMAP_ROW_SIZE (MATRIX_COLS * SINGLE_KEY_SIZE)
#define KEYMAP_LAYER_SIZE (MATRIX_ROWS * KEYMAP_ROW_SIZE)

enum storage_type {
    STORAGE_KEYMAP, // Keymap
    STORAGE_MACRO, // 自定义宏
    STORAGE_CONFIG, // 配置区域
    STORAGE_ENCODER //旋钮区域
};

#define RGB_MATRIX_CONFIG_SIZE 5
#define RGB_MATRIX_CONFIG_P    0

#define RGB_LIGHT_CONFIG_SIZE 4
#define RGB_LIGHT_CONFIG_P    RGB_MATRIX_CONFIG_SIZE


#ifdef KEYMAP_STORAGE
extern uint8_t keymap_block[];
#endif
#ifdef MACRO_STORAGE
extern uint8_t macro_block[];
#endif
#ifdef CONFIG_STORAGE
extern uint8_t config_block[];
#endif
#ifdef ENCODER_ENABLED
extern uint8_t encoder_block[];
#endif

uint16_t storage_read_data(enum storage_type type, uint16_t offset, uint16_t len, uint8_t *data);

uint16_t storage_write_data(enum storage_type type, uint16_t offset, uint16_t len, uint8_t *data);

void storage_read(uint8_t mask);

void storage_write(uint8_t mask);
