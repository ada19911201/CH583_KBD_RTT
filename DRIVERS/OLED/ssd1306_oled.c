#include <stdint.h>
#include <string.h>
#include "i2c_master.h"
#include "oled_graph.h"
#include "task_config.h"
#include "ssd1306_oled.h"
#include "icons.h"
#include "wait_api.h"
#include "nrf_log.h"


static uint8_t cmd_data_buff[2];

/**
 * @brief SSD1306的显示屏初始化命令
 * 
 */
const uint8_t ssd1306_init_commands[] = {
    SSD1306_DISPLAYOFF, /* display off */
    SSD1306_SETLOWCOLUMN, /* set lower column address */
    SSD1306_SETHIGHCOLUMN, /* set higher column address */
    SSD1306_SETSTARTLINE, /* set display start line */
    SSD1306_PAGESTARTADDR, /* set page address */
    SSD1306_SETCONTRAST, /* contract control */
    0xff, /* 128 */
#ifdef SSD1306_ROTATE_180
    SSD1306_SEGREMAP_RESET, /* set segment remap */
#else
    SSD1306_SEGREMAP_INVERSE, /* set segment remap */
#endif
    SSD1306_NORMALDISPLAY, /* normal / reverse */
    SSD1306_SETMULTIPLEX, /* multiplex ratio */
    0x1F, /* duty = 1/32 */
#ifdef SSD1306_ROTATE_180
    SSD1306_COMSCANINC, /* Com scan direction */
#else
    SSD1306_COMSCANDEC,
#endif
    SSD1306_SETDISPLAYOFFSET, /* set display offset */
    0x00,
    SSD1306_SETDISPLAYCLOCKDIV, /* set osc division */
    0x80,
    SSD1306_SETPRECHARGE, /* set pre-charge period */
    0x1f,
    SSD1306_SETCOMPINS, /* set COM pins */
    0x00,
    SSD1306_SETVCOMDETECT, /* set vcomh */
    0x40,
    SSD1306_CHARGEPUMP, /* set charge pump enable */
    0x14,
    SSD1306_COLUMNADDR,
    0x00,
    0x7F,
    SSD1306_PAGEADDR,
    0x00,
    0x03,
    SSD1306_DISPLAYON, /* display ON */
};

uint8_t ssd1306_display_buffer[128 * 4] = SSD1306_INIT_BUFF;

/**
 * @brief 发送命令给SSD1306
 * 
 * @param len 长度
 * @param data 数据指针。
 */
static bool ssd1306_write_cmd(uint16_t len, const uint8_t *data)
{
    cmd_data_buff[0] = IS_CMD;
    for (size_t i = 0; i < len; i++)
    {    
        cmd_data_buff[1] = *(data++);
        if(i2c_transmit( SSD1306_ADDR << 1, cmd_data_buff, 2, false) != 0){
            NRF_LOG_INFO("i2c_transmit_fail");
        return false;
        }
    }
    return true;
}
/**
 * @brief 发送数据给SSD1306
 * 
 * @param len 长度
 * @param data 数据指针。
 */
static bool ssd1306_write_data(uint16_t len, const uint8_t* data)
{
    cmd_data_buff[0] = IS_DATA;
    for (size_t i = 0; i < len; i++)
    {
        cmd_data_buff[1] = *(data++);
        if(i2c_transmit( SSD1306_ADDR << 1, cmd_data_buff, 2, false) != 0){
        return false;
        }
    }
    return true;
}


/**
 * @brief 初始化OLED屏幕
 * 
 */
void ssd1306_oled_init()
{
    ssd1306_write_cmd(sizeof(ssd1306_init_commands), (uint8_t*)ssd1306_init_commands);
}

/**
 * @brief 显示指定行的Buff
 * 
 * @param row 行号，0-3
 * @param col_start 列开始Index，0-127
 * @param len 显示长度
 */
void ssd1306_show_buff(uint8_t row, uint8_t col_start, uint8_t len)
{
    if (row >= SSD1306_ROWS)
        return;
    if (col_start + len > SSD1306_COLS)
        len = SSD1306_COLS - col_start;

    uint8_t commands[] = { 0xB0 + row, 0x00 + (col_start & 0xF), 0x10 + ((col_start >> 4) & 0xF) };
    ssd1306_write_cmd(sizeof(commands), commands);
    ssd1306_write_data(len, &ssd1306_display_buffer[row * 128 + col_start]);
}


/**
 * @brief OLED显示缓存脏标记
 * 
 */
bool ssd1306_buff_dirty[SSD1306_ROWS];
/**
 * @brief 显示Buff里面的所有内容
 * 
 */
void ssd1306_show_all()
{
    NRF_LOG_INFO("ssd1306_show_all");
    for (uint8_t i = 0; i < SSD1306_ROWS; i++) {
        ssd1306_show_buff(i, 0, SSD1306_COLS);
    }
}

/**
* @brief 清空Buff
* 
*/
void ssd1306_clr()
{
    memset(ssd1306_display_buffer, 0, sizeof(ssd1306_display_buffer));
}

/**
 * @brief 进入睡眠状态
 * 
 */
void ssd1306_sleep()
{
    uint8_t cmd = SSD1306_DISPLAYOFF;
    ssd1306_write_cmd(1, &cmd);
    NRF_LOG_INFO("ssd1306_sleep");
}

/**
* @brief 退出睡眠状态 
* 
*/
void ssd1306_wake()
{
    uint8_t cmd = SSD1306_DISPLAYON;
    ssd1306_write_cmd(1, &cmd);
}

/**
 * @brief 更新状态栏
 * 
 */
void update_status_bar(uint8_t row, uint8_t col, enum icon_index type)
{
    // oled_clear_row(row);
    // oled_clear_row(row + 1);
    oled_draw_icon(row, col, Icons[type]);
}

void update_status_bar_notify(uint8_t row, uint8_t col, enum icon_index type)
{
    update_status_bar(row, col, type);
    BaseType_t xReturn = pdPASS;
    xReturn = xTaskNotify(m_ssd1306_task_thread, SSD1306_UPDATA, eNoAction);
    if (xReturn != pdPASS)
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
}
void ssd1306_task_thread (void *p_context)
{
    BaseType_t xReturn = pdTRUE;
    while (1)
    {
        xReturn = xTaskNotifyWait(0x0,              //进入函数的时候不清除任务 bit
                                  0x0,              //退出函数的时候不清除所有的 bit
                                  NULL,             //保存任务通知值NULL
                                  portMAX_DELAY);   //阻塞时间
        if (pdTRUE == xReturn)
        {
            ssd1306_show_all();
            wait_ms(SSD1306_TASK_TIMERS);
        }
    }
}