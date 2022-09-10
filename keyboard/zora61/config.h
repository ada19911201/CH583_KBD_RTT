/*
Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2019 Jim Jiang <jim@lotlab.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

/* USB Device descriptor parameter */
#define VENDORS_ID 0x0520 /* USB VID */
#define PRODUCT_ID 0x0061 /* USB PID */
#define DEVICE_VER 0x0100 /* 硬件版本 */
#define MANUFACTURER "Zorokb" /* 硬件制造商，用于蓝牙显示 */
#define PRODUCT "Zoro_61"/* 硬件名词，用于蓝牙显示 */
#define MACADDR_SEPRATOR ' ' /* 蓝牙名称后地址的分隔符。若不设置则不显示蓝牙名称后面的地址 */

/* USB HID report parameter */
#define KEYBOARD_EPSIZE 8 /* 键盘上传端点大小，请不要修改 */
#define NKRO_EPSIZE 28 /* 键盘NKRO端点大小，请不要修改 */

/* key matrix size */
#define MATRIX_ROWS 5 /* 硬件阵列行数 */
#define MATRIX_COLS 14 /* 硬件阵列列数 */

/* define if matrix has ghost */
// #define MATRIX_HAS_GHOST /* 按键阵列是否出现Ghost Key，若没有加二极管则需要启用这个项目 */

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5 /* 硬件消抖次数 */

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

#ifdef COMMAND_ENABLE
/* key combination for command */
#define IS_COMMAND() (     keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)))
#endif

// 键盘省电参数
#define SLEEP_OFF_TIMEOUT           600             // 键盘闲置多久后转入自动关机 (s)
#define MATRIX_SCAN_DELAY_CYCLE     36              // 等待IO稳定时间(阻塞模式)
#define BLE_FAST_AD_INTERVAL        80              // 快速广播间隔(0.625ms)
#define BLE_FAST_AD_DURATION        60              // 快速广播时间(s)
#define LED_AUTOOFF_TIME            0               // LED自动熄灭时长(ms)，设为0则不自动熄灭

//#define PASSKEY_REQUIRED                          // 需要输入配对码
#define HIGH_TX_POWER                               // 更改发射功率到+8dBm
#define MULTI_DEVICE_SWITCH                         // 启用多设备切换
#define ENABLE_WATCHDOG                             // 启用看门狗
#define KEYMAP_STORAGE                              // 启用keymap存储
#define CONFIG_STORAGE                              // 启用配置存储功能
#define MACRO_STORAGE                              // 启用配置存储功能

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
//#define NO_ACTION_MACRO
//#define NO_ACTION_FUNCTION

// 旋钮配置
//#define ROTARY_ENCODER_A 13 //13
//#define ROTARY_ENCODER_B 14 //14
//定义旋钮映射的矩阵按键位置
//#define ROTARY_ENCODER_POS 5,5   // 旋钮正向按钮映射(不应超过最大行数)
//#define ROTARY_ENCODER_NEG 5,5   // 旋钮负向按钮映射(不应超过最大列数)


// LED 配置
#ifdef STATUS_RGB_ENABLE
#define LEDSIGNAL_DI_PIN 5 // 21
#define LEDSIGNAL_NUM    4
#else
#define BLE_LED     5 // 21
#define USB_LED     5
#define CAPS_LED    5
#endif
// ws2812 RGB 配置
#define RGBLED_NUM 8 // 8
#define RGB_DI_PIN 29 // 10
#define RGB_PWR_PIN 41 // P-mos
//#define RGB_PWR_PIN_REVERSE 20 // N-mos


// This is a 7-bit address, that gets left-shifted and bit 0
// set to 0 for write, 1 for read (as per I2C protocol)
// The address will vary depending on your wiring:
// 0b1110100 AD <-> GND
// 0b1110111 AD <-> VCC
// 0b1110101 AD <-> SCL
// 0b1110110 AD <-> SDA
#define DRIVER_ADDR_1 0b1010000
#define DRIVER_COUNT 1
#define DRIVER_1_LED_TOTAL 64 
#define DRIVER_2_LED_TOTAL 0
#define DRIVER_LED_TOTAL (DRIVER_1_LED_TOTAL + DRIVER_2_LED_TOTAL)
//rgb—矩阵
#define RGB_MATRIX_STARTUP_ENABLE  1    //默认开启还是关闭EGB轴灯
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define ENABLE_RGB_MATRIX_TYPING_HEATMAP
// #define RGB_MATRIX_KEYRELEASES // reacts to keyreleases (instead of keypresses)释放
#define RGB_MATRIX_KEYPRESSES // reacts to keypresses 按下
#define RGB_DISABLE_AFTER_TIMEOUT 0 // number of ticks to wait until disabling effects
#define RGB_DISABLE_WHEN_USB_SUSPENDED false // turn off effects when suspended
#define RGB_MATRIX_LED_PROCESS_LIMIT (DRIVER_LED_TOTAL + 4) / 5 // limits the number of LEDs to process in an animation per task run (increases keyboard responsiveness)
#define RGB_MATRIX_LED_FLUSH_LIMIT 16 // limits in milliseconds how frequently an animation will update the LEDs. 16 (16ms) is equivalent to limiting to 60fps (increases keyboard responsiveness)
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 200 // limits maximum brightness of LEDs to 200 out of 255. If not defined maximum brightness is set to 255
#define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_CYCLE_LEFT_RIGHT // Sets the default mode, if none has been set
#define RGB_MATRIX_STARTUP_HUE 0 // Sets the default hue value, if none has been set
#define RGB_MATRIX_STARTUP_SAT 255 // Sets the default saturation value, if none has been set
#define RGB_MATRIX_STARTUP_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS // Sets the default brightness value, if none has been set
#define RGB_MATRIX_STARTUP_SPD 127 // Sets the default animation speed, if none has been set

// 电量检测配置 Pin 2
#define BATTERY_ADC_PIN NRF_SAADC_INPUT_AIN6 // 电量检测引脚


// 按键阵列配置
static const uint8_t MATRIX_ROW_PINS[MATRIX_ROWS] = { 0, 1, 2, 3, 4 };
static const uint8_t MATRIX_COL_PINS[MATRIX_COLS] = { 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 31, 32, 33, 34};
#define COL_IN // 键盘阵列的二极管方向是从COL->ROW

#define LED_POSITIVE // LED上拉驱动

#endif