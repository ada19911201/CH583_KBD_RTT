#pragma once

#include "config.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


enum status_rgb_id{
    SIGNAL_MODE,
    WIRE_MODE,
    LAYER_STATUS,
    USB_MODE
};

enum wire_mode{
    NO_LINK,
    BLE_LINK,
    NRF_LINK
};
enum layer_status{
    LEYER_0,
    LEYER_1,
    LEYER_2,
    LEYER_3,
    LEYER_4,
    LEYER_5,
    LEYER_6,
    LEYER_7,
};
enum usb_mode{
    USB_DISABLE,
    USB_ENABLE
};

extern uint32_t led_counter ;
#ifdef STATUS_RGB_ENABLE
void all_state_led_thread(void *arg);
#endif
void keyboard_led_timeout(void);
void led_check(uint8_t id,uint8_t value);
void keyboard_led_init(void);
void led_set(uint8_t keyboard_led);
void turn_off_led_device(void);