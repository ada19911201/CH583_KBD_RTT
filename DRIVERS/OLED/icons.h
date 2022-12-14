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

#ifndef  ICONS_H
#define  ICONS_H

#include "image_header.h"

#ifdef  BOY
static const uint8_t image_data_open[32] =
    {//Girl图标
        0xFF,0xFF,0x7F,0x7F,0xBF,0xBF,0xBF,0x7F,
        0x7D,0xBD,0xDD,0xED,0xF5,0xF9,0x81,0xFF,
        0xFF,0xF1,0xCE,0xDF,0xBF,0xBF,0xBF,0xDF,
        0xCE,0xF1,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
        };
const tImage  Open = {image_data_open, 16, 16, 8};
#else
static const uint8_t image_data_open[32] =
    {//Boy图标
        0xFF,0xFF,0xFF,0xCF,0xB7,0x7B,0xFB,0xFD,
        0xFD,0xFB,0x7B,0xB7,0xCF,0xFF,0xFF,0xFF,
        0xFF,0xFF,0xFF,0xEF,0xEF,0xEF,0xEE,0x80,
        0x80,0xEE,0xEF,0xEF,0xEF,0xFF,0xFF,0xFF
        };
const tImage Open = {image_data_open, 16, 16,
                           8};
#endif
//电量不满10%图标
static const uint8_t image_data_Batt_0[64] = 
{
        0x00, 0xFE, 0x02, 0xFA, 0xFA, 0x02, 0x02, 0x02,
        0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
        0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
        0x02, 0x02, 0x02, 0xFE, 0x20, 0x20, 0xE0, 0x00,
        0x00, 0x7F, 0x40, 0x5F, 0x5F, 0x40, 0x40, 0x40,
        0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
        0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
        0x40, 0x40, 0x40, 0x7F, 0x04, 0x04, 0x07, 0x00
    };
const tImage Batt_0 = {image_data_Batt_0, 32, 16,8};

//电量100%
static const uint8_t image_data_Batt_100[64] = {

    0x00, 0xFE, 0xFE, 0x06, 0xF6, 0x16, 0x56, 0x56,
    0x56, 0xF6, 0x06, 0xF6, 0x56, 0x56, 0x56, 0x16,
    0xF6, 0x06, 0xF6, 0x16, 0xD6, 0xD6, 0x16, 0xF6,
    0x06, 0xFE, 0xFE, 0x30, 0x30, 0xF0, 0xF0, 0x00,
    0x00, 0x7F, 0x7F, 0x60, 0x6F, 0x6B, 0x6B, 0x6B,
    0x68, 0x6F, 0x60, 0x6F, 0x68, 0x6B, 0x6B, 0x6B,
    0x6F, 0x60, 0x6F, 0x68, 0x6B, 0x6B, 0x68, 0x6F,
    0x60, 0x7F, 0x7F, 0x0C, 0x0C, 0x0F, 0x0F, 0x00};

const tImage Batt_100 = {image_data_Batt_100, 32, 16,
                         8};

//充电图标
static const uint8_t image_data_Charge[16] = 
{
        0x60,0xE0,0x3E,0x60,0x60,0x3E,0xE0,0x60,
        0x00,0x07,0x06,0x7C,0x7C,0x46,0x47,0x00
        };
const tImage Charge = {image_data_Charge, 8, 16,
                       8};

//蓝牙图标
static const uint8_t image_data_Bluetooth[16] = 
{
    0x18, 0x30, 0x60, 0xfe, 0x8c, 0xd8, 0x70, 0x20,
    0x18, 0x0c, 0x06, 0x7f, 0x31, 0x1b, 0x0e, 0x04
    };
const tImage Bluetooth = {image_data_Bluetooth, 8, 16,
                          8};

//ble_1
static const uint8_t image_data_ble_one[16] =
    {
        0xFF,0xFF,0xFB,0x01,0x01,0xFF,0xFF,0xFF,
        0xFF,0xBF,0x9F,0x80,0x80,0x9F,0xBF,0xFF
        };
const tImage Blue_one = {image_data_ble_one, 8, 16,
                          8};
//ble_2
static const uint8_t image_data_ble_two[16] =
    {
        0xFF,0xF3,0xF9,0x7D,0x3D,0x99,0xC3,0xFF,
        0xFF,0xC3,0x99,0xBC,0xBF,0x9F,0xCF,0xFF
        };
const tImage Blue_two = {image_data_ble_two, 8, 16,
                          8};
//ble_3
static const uint8_t image_data_ble_three[16] =
    {
        0xFF,0xF3,0x79,0x7D,0x3D,0x99,0xC3,0xFF,
        0xFF,0xCF,0x9E,0xBE,0xBC,0x99,0xC3,0xFF
        };
const tImage Blue_three = {image_data_ble_three, 8, 16,
                          8};

//2.4g图标
static const uint8_t image_data_Gazell[32] = {

    0x00, 0x20, 0xB0, 0x98, 0xD8, 0x4C, 0x6C, 0x66,
    0x66, 0x6C, 0x4C, 0xD8, 0x98, 0xB0, 0x20, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x06, 0x13, 0x3B,
    0x3B, 0x13, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00};
const tImage Gazell = {image_data_Gazell, 16, 16,
                       8};

//usb图标
static const uint8_t image_data_USB[32] = 
    {
        0x00, 0x80, 0xC0, 0xC0, 0x80, 0xC0, 0xA0, 0x90,
        0x88, 0xBC, 0x88, 0x88, 0x80, 0x80, 0x80, 0x00,
        0x00, 0x01, 0x03, 0x03, 0x01, 0x01, 0x01, 0x03,
        0x05, 0x09, 0x11, 0x39, 0x11, 0x11, 0x11, 0x00
        };
const tImage USB = {image_data_USB, 16, 16,
                    8};

//numlock和全键无冲图标
static const uint8_t image_data_NumLock_Nkro[16] = 
    {
        0xFF,0x03,0x01,0x0F,0xFF,0x01,0x03,0xFF,
        0xFF,0xC0,0x80,0xFE,0xE0,0x80,0xC0,0xFF
        };
const tImage NumLock_Nkro = {image_data_NumLock_Nkro, 8, 16,
                        8};
//capslock
static const uint8_t image_data_CapsLock[16] = 
    {
        0xFF,0x07,0xE3,0xF9,0xFD,0xF9,0xF3,0xFF,
        0xFF,0xF0,0xC7,0x9F,0xBF,0x9F,0xCF,0xFF
        };
const tImage CapsLock = {image_data_CapsLock, 8, 16,
    8};
//scrolllock
static const uint8_t image_data_ScrollLock_Slow[16] =
    {
        0xFF,0xC3,0xB9,0x7D,0xFD,0xF9,0xF3,0xFF,
        0xFF,0xCF,0x9F,0xBF,0xBE,0x9D,0xC3,0xFF
        };
const tImage ScrollLock_Slow = {image_data_ScrollLock_Slow, 8, 16,
                           8};
//飞机符号
static const uint8_t image_data_Air[16] =
    {
        0x7F,0x3F,0x1F,0x01,0x01,0x1F,0x3F,0x7F,
        0xFF,0xBF,0x9F,0x80,0x80,0x9F,0xBF,0xFF
        };
const tImage Air = {image_data_Air, 8, 16,
                           8};

//6键无冲图标
static const uint8_t image_data_Sixrko[16] =
    {
        0x00,0xFC,0xFE,0x86,0x86,0x86,0x0C,0x00,
        0x00,0x3F,0x7F,0x61,0x61,0x7F,0x3F,0x00
        };
const tImage   Sixrko = {image_data_Sixrko, 8, 16,
                           8};


//MODE:图标反色
static const uint8_t image_data_Mode[24] =
    {
        0xFF,0x01,0x01,0x0F,0x3F,0x3F,0x0F,0x01,
        0x01,0xFF,0xCF,0xCF,0xFF,0x80,0x80,0xFE,
        0xF8,0xF8,0xFE,0x80,0x80,0xFF,0xF3,0xF3
        };
const tImage   Mode = {image_data_Mode, 12, 16,
                           8};
                           
//LAYER:图标反色
static const uint8_t image_data_Layer[24] =
    {
        0xFF,0x01,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,
        0xFF,0xCF,0xCF,0xFF,0xFF,0x80,0x80,0x9F,
        0x9F,0x9F,0x9F,0x9F,0xFF,0xF3,0xF3,0xFF
        };
const tImage   Layer = {image_data_Layer, 12, 16,
                           8};
                
//modfiy:图标反色
static const uint8_t image_data_Modfiy[24] =
    {
        0xFF,0x01,0x01,0x79,0x79,0x79,0x79,0x79,
        0xFF,0xCF,0xCF,0xFF,0xFF,0x80,0x80,0xFE,
        0xFE,0xFE,0xFE,0xFE,0xFF,0xF3,0xF3,0xFF
        };
const tImage   Modfiy = {image_data_Modfiy, 12, 16,
                           8};

//KEY:图标反色
static const uint8_t image_data_PressKey[24] =
    {
        0xFF,0x01,0x01,0x3F,0x9F,0xCF,0xE7,0xF1,
        0xFF,0xCF,0xCF,0xFF,0xFF,0x80,0x80,0xFC,
        0xF9,0xF3,0xE7,0x8F,0xFF,0xF3,0xF3,0xFF
        };
const tImage   PressKey = {image_data_PressKey, 12, 16,
                           8};

//16*16空白
static const uint8_t image_data_Icon16x16[32] =
    {
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        };
const tImage   Icon16x16 = {image_data_Icon16x16, 16, 16,
                           8};

//8*16空白
static const uint8_t image_data_Icon8x16[16] =
    {
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        };
const tImage   Icon8x16 = {image_data_Icon16x16, 8, 16,
                           8};

//FAST快速广告
static const uint8_t image_data_Fast[16] =
    {
        0x00,0xFE,0xFE,0x86,0x86,0x86,0x86,0x00,
        0x00,0x7F,0x7F,0x01,0x01,0x01,0x01,0x00
        };
const tImage   Fast = {image_data_Fast, 8, 16,
                           8};
const tImage *Icons[] = {
    &Open,
    &Batt_0,
    &Batt_100,
    &Charge,
    &Bluetooth,
    &Blue_one,
    &Blue_two,
    &Blue_three,
    &Gazell,
    &USB,
    &NumLock_Nkro,
    &CapsLock,
    &ScrollLock_Slow,
    &Air,
    &Sixrko,
    &Mode,
    &Layer,
    &Modfiy,
    &PressKey,
    &Icon16x16,
    &Icon8x16,
    &Fast,
    };

#endif