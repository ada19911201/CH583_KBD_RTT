#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "host.h"
#include "host_driver.h"


struct device_led{
    uint8_t keyboard_led_stats_usb ; //usb指示灯
    uint8_t keyboard_led_stats_ble ; //蓝牙指示灯
    uint8_t keyboard_led_stats_rf ;  //2.4g指示灯
};
extern struct device_led usb_ble_led;
/**
 * @brief TMK使用的driver
 * 
 */
extern host_driver_t driver;

uint8_t keyboard_leds(void);
void send(uint8_t index, uint8_t len, uint8_t* keys);

/**
 * @brief 发送数据的类型
 * 
 */
enum packet_type {
    PACKET_KEYBOARD = 0,
    PACKET_MOUSE = REPORT_ID_MOUSE,
    PACKET_SYSTEM = REPORT_ID_SYSTEM,
    PACKET_CONSUMER = REPORT_ID_CONSUMER,
    PACKET_NKRO = 0x80,
};

/**
 * @brief 内部多模的driver
 * 
 */
struct host_driver {
    /**
     * @brief 获取当前键盘LED
     * 
     */
    uint8_t (*keyboard_leds)(void);
    /**
     * @brief 发送按键数据包
     * 
     */
    void (*send_packet)(enum packet_type type, uint8_t len, uint8_t* data);
    /**
     * @brief 队列是否为空
     * 
     */
    bool (*queue_empty)(void);
    /**
     * @brief 当前此驱动是否工作中
     * 
     */
    bool (*driver_working)(void);
};

