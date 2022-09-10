#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "keyboard_event.h"

//定义宏播放标志
extern bool macro_send_end;

enum user_event{

    USER_EVT_TICK,//键盘内置Tick (1s) 触发

    BLE_STATE_IDLE, // 蓝牙处于空闲状态（FAST_ADV->SLOW_ADV->IDLE)
    BLE_STATE_DISCONNECT, // 蓝牙连接断开
    BLE_STATE_CONNECTED,// 蓝牙已连接

    SLOW_SCAN,
    SYS_TIMEOUT,    // 按键超时自动休眠
    SYS_SLEEP,    // 手动关机休眠
    SYS_SETOFF,    // 手动关机休眠，并禁用按键唤醒

    LED_TIMEOUT,    // 指示灯超时自动休眠

    INTERNAL_EVT_GATTS_TX_COMPLETE, // 蓝牙GATTS发送成功（启用加密情况下表示连接成功）
    PASSKEY_STATE_REQUIRE, // 请求配对码
    PASSKEY_STATE_INPUT, // 配对码输入
    PASSKEY_STATE_SEND, // 配对码已发送

    RGBLIGHT_TOG,
    RGBMATRIX_TOG,
    FUNCTION_NKRO,       //NKRO切换
    FUNCTION_WIRELESS_USB,
    FUNCTION_BLE_ONE,
    FUNCTION_BLE_TWO,
    FUNCTION_BLE_THR,
    FUNCTION_BLE_RESET,
    FUNCTION_DFU,        //跳转Bootloader
    FUNCTION_24G_DFU,   //24g接收器进入DFU模式
    FUNCTION_RESET_KB,      //重置键盘
    FUNCTION_BOND,       //清空绑定
    RGB_TOP_OPEN,
    RGB_TOP_CLOSE,
    RGB_BOT_OPEN,
    RGB_BOT_CLOSE,
    FUNCTION_MACRO_0,
    FUNCTION_MACRO_1,
    FUNCTION_MACRO_2,
    FUNCTION_MACRO_3,
    FUNCTION_MACRO_4,
    FUNCTION_MACRO_5,
    FUNCTION_MACRO_6,
    FUNCTION_MACRO_7,
    FUNCTION_MACRO_8,
    FUNCTION_MACRO_9,
    FUNCTION_MACRO_10,
    FUNCTION_MACRO_11,
    FUNCTION_MACRO_12,
    FUNCTION_MACRO_13,
    FUNCTION_MACRO_14,
    FUNCTION_MACRO_15,
    
    FUNCTION_MACRO_16,
    FUNCTION_MACRO_17,
    FUNCTION_MACRO_18,
    FUNCTION_MACRO_19,
    FUNCTION_MACRO_20,
    FUNCTION_MACRO_21,
    FUNCTION_MACRO_22,
    FUNCTION_MACRO_23,
    FUNCTION_MACRO_24,
    FUNCTION_MACRO_25,
    FUNCTION_MACRO_26,
    FUNCTION_MACRO_27,
    FUNCTION_MACRO_28,
    FUNCTION_MACRO_29,
    FUNCTION_MACRO_30,
    FUNCTION_MACRO_31,
    SW_TO_BLE,
    SW_TO_24G,
    SW_TO_USB,
    PRINTT_BAT,
    PIPE_24G_0,
    PIPE_24G_1,
    PIPE_24G_2,
    PIPE_24G_3,
    PIPE_24G_4,
    PIPE_24G_5,
    PIPE_24G_6,
    PIPE_24G_7,
    CHANNEL_24G_0,
    CHANNEL_24G_1,
    CHANNEL_24G_2,
    CHANNEL_24G_3,
    CHANNEL_24G_4,
    ENCODER_LEFT,
    ENCODER_RIGHT,
    RELEASE
};
void send_event_message(enum user_event type);
void kbd_event_processing( uint32_t receive_event);
