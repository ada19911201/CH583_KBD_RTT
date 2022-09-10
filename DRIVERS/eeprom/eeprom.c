#include "eeprom.h"
#include "z_util.h"
#include "ISP583.h"
// keymap
#include "keymap.h"
#include "keymap_common.h"
#include <string.h>

#include "xprintf.h"


#define GET_WORD(_i) ((_i + 3) / 4)

#ifdef KEYMAP_STORAGE
#define KEYMAP_SIZE_WORD    GET_WORD(KEYMAP_LAYER_SIZE *MAX_LAYER)
#define STORAGE_KEYMAP_P    0x0000
uint8_t keymap_block[KEYMAP_SIZE_WORD * 4];
static bool keymap_valid = false;
static void check_keymap()
{
    bool flag = false;
    for (uint16_t i = 0; i < KEYMAP_LAYER_SIZE * MAX_LAYER; i++)
    {
        flag |= (keymap_block[i] != 0);
        if (flag)
            break;
    }
    keymap_valid = flag;
}


extern const action_t actionmaps[][MATRIX_ROWS][MATRIX_COLS];

action_t action_for_key(uint8_t layer, keypos_t key)
{
    if (layer >= KEYMAP_LAYER_SIZE || key.col >= MATRIX_COLS || key.row >= MATRIX_ROWS)
    {
        action_t action = AC_NO;
        return action;
    }
    if (keymap_valid)
    {
        action_t action;
        uint16_t index = layer * KEYMAP_LAYER_SIZE + key.row * KEYMAP_ROW_SIZE + key.col * 2;
        action.code = UINT16_READ(keymap_block, index);
        return action;
    }
    else
    {
        return actionmaps[layer][key.row][key.col];
    }
}
#endif

#ifdef MACRO_STORAGE
#define MACRO_BLOCK_SIZE_WORD GET_WORD(MAX_MACRO_SIZE)
#define STORAGE_MACRO_P     STORAGE_KEYMAP_P+KEYMAP_SIZE_WORD*4
uint8_t macro_block[MACRO_BLOCK_SIZE_WORD * 4];
#endif

#ifdef CONFIG_STORAGE
#define CONFIG_BLOCK_SIZE_WORD 6
#define STORAGE_CONFIG_P    STORAGE_MACRO_P+MACRO_BLOCK_SIZE_WORD*4
uint8_t config_block[CONFIG_BLOCK_SIZE_WORD*4];
#endif

#ifdef ENCODER_ENABLED
#define ENCODER_BLOCK_SIZE_WORD 4
#define STORAGE_ENCODER_P   STORAGE_CONFIG_P+ENCODER_BLOCK_SIZE_WORD * 4
uint8_t encoder_block[ENCODER_BLOCK_SIZE_WORD * 4];
#endif
/**
 * @brief 获取指定类型的内存区域的指针和大小
 * 
 * @param type 存储类型
 * @param pointer 目标指针
 * @return uint16_t 存储空间大小。0则表示不支持。
 */
static uint16_t storage_get_data_pointer(enum storage_type type, uint8_t **pointer)
{
    switch (type)
    {
#ifdef KEYMAP_STORAGE
    case STORAGE_KEYMAP:
        *pointer = keymap_block;
        return KEYMAP_SIZE_WORD * 4;
        break;
#endif
#ifdef MACRO_STORAGE
    case STORAGE_MACRO:
        *pointer = macro_block;
        return MACRO_BLOCK_SIZE_WORD * 4;
        break;
#endif
#ifdef CONFIG_STORAGE
    case STORAGE_CONFIG:
        *pointer = config_block;
        return CONFIG_BLOCK_SIZE_WORD * 4;
        break;
#endif
#ifdef ENCODER_ENABLED
    case STORAGE_ENCODER:
        *pointer = encoder_block;
        return ENCODER_BLOCK_SIZE_WORD * 4;
        break;
#endif
    default:
        return 0;
    }
}

uint16_t storage_read_data(enum storage_type type, uint16_t offset, uint16_t len, uint8_t *data)
{
    uint8_t *pointer = 0;
    uint16_t size = storage_get_data_pointer(type, &pointer);

    if (pointer == 0)
        return 0;

    if (size < len + offset)
        len = size - offset;

    memcpy(data, &pointer[offset], len);
    return len;
}


//将数据写到内存中
uint16_t storage_write_data(enum storage_type type, uint16_t offset, uint16_t len, uint8_t *data)
{
    uint8_t *pointer = 0;
    uint16_t size = storage_get_data_pointer(type, &pointer);

    if (pointer == 0)
        return 0;

    if (size < len + offset)
        len = size - offset;

    memcpy(&pointer[offset], data, len);

    if (type == STORAGE_KEYMAP)
        check_keymap();
#ifdef ENCODER_ENABLED
    if (type == STORAGE_ENCODER)
        check_encoder();
#endif
    return len;
}

static void eeprom_read(uint32_t StartAddr, void *Buffer, uint32_t Length){
    EEPROM_READ(StartAddr, Buffer, Length);
}

void storage_read(uint8_t mask)
{
#ifdef KEYMAP_STORAGE
    if (mask & 0x01)
    {
        eeprom_read(STORAGE_KEYMAP_P, keymap_block,KEYMAP_SIZE_WORD * 4);
        check_keymap();
    }
#endif
#ifdef MACRO_STORAGE
    if (mask & 0x02)
    {
        eeprom_read(STORAGE_MACRO_P, macro_block,MACRO_BLOCK_SIZE_WORD * 4);
    }
#endif
#ifdef CONFIG_STORAGE
    if (mask & 0x04)
    {
        eeprom_read(STORAGE_CONFIG_P, config_block,CONFIG_BLOCK_SIZE_WORD * 4);
    }
#endif
#ifdef ENCODER_ENABLED
    if (mask & 0x08)
    {
        eeprom_read(STORAGE_ENCODER_P, encoder_block,ENCODER_BLOCK_SIZE_WORD * 4);
    }
#endif
}

static void eeprom_write(uint32_t StartAddr, void *Buffer, uint32_t Length){
    EEPROM_ERASE(StartAddr, Length);
    EEPROM_WRITE(StartAddr, Buffer, Length);
}

void storage_write(uint8_t mask)
{
#ifdef KEYMAP_STORAGE
    if (mask & 0x01)
    {
        eeprom_write(STORAGE_KEYMAP_P, keymap_block,KEYMAP_SIZE_WORD * 4);
    }
#endif
#ifdef MACRO_STORAGE
    if (mask & 0x02)
    {
        eeprom_write(STORAGE_MACRO_P, macro_block,MACRO_BLOCK_SIZE_WORD * 4);
    }
#endif
#ifdef CONFIG_STORAGE
    if (mask & 0x04)
    {
        eeprom_write(STORAGE_CONFIG_P, config_block,CONFIG_BLOCK_SIZE_WORD * 4);
    }
#endif
#ifdef ENCODER_ENABLED
    if (mask & 0x08)
    {
        eeprom_write(STORAGE_ENCODER_P, encoder_block,ENCODER_BLOCK_SIZE_WORD * 4);
    }
#endif
}