#include "led.h"
#include "all_state_led.h"
#include "ledsignal.h"
#include "keyboard_event.h"
#include "process_record_kb.h"
#include "wait_api.h"
#include "nrf_gpio.h"
#include "host.h"
#include "usb_host.h"
#include "ble_bas_service.h"
#include "task_config.h"
#include "config.h"
#include "status_task.h"
#include "util.h"
#include "action_layer.h"
#ifdef STATUS_RGB_ENABLE
//状态灯数组
LED_TYPE led_state[LEDSIGNAL_NUM];

LED_TYPE led_dis_status[LEDSIGNAL_NUM];
uint32_t led_counter = 0;

void led_check(uint8_t id, uint8_t value)
{
    switch (id)
    {
    case SIGNAL_MODE:
        led_state[id].r = value & (1 << 0) ? 1 : 0;
        led_state[id].g = value & (1 << 1) ? 1 : 0;
        led_state[id].b = value & (1 << 2) ? 1 : 0;
    case WIRE_MODE:
        switch (value)
        {
        case NO_LINK:
            led_state[id].r = 0;
            led_state[id].g = 0;
            led_state[id].b = 0;
            break;
        case BLE_LINK:
            led_state[id].r = (switch_id == 0) ? 1 : 0;
            led_state[id].g = (switch_id == 1) ? 1 : 0;
            led_state[id].b = (switch_id == 2) ? 1 : 0;
            break;
        case NRF_LINK:
            led_state[id].r = 1;
            led_state[id].g = 1;
            led_state[id].b = 1;
            break;
        default:
            break;
        }
        break;
    case LAYER_STATUS:
        led_state[id].r = 1 & (biton32(layer_state));
        led_state[id].g = (2 & (biton32(layer_state))) >> 1;
        led_state[id].b = (4 & (biton32(layer_state))) >> 2;
        break;
    case USB_MODE:
        led_state[id].r = (value) ? 1 : 0;
        led_state[id].g = (value) ? 1 : 0;
        led_state[id].b = (value) ? 1 : 0;
        break;
    default:
        break;
    }
}

void keyboard_led_init(void)
{
    for (uint8_t i = 0; i < LEDSIGNAL_NUM; i++)
    {
        led_state[i].r = 0;
        led_state[i].g = 0;
        led_state[i].b = 0;
        led_dis_status[i].r = 0;
        led_dis_status[i].g = 0;
        led_dis_status[i].b = 0;
    }
    led_setleds(led_state, LEDSIGNAL_NUM);
}

void keyboard_led_timeout(void){
    led_setleds(led_dis_status, LEDSIGNAL_NUM);
}
void turn_off_led_device(void)
{
    keyboard_led_init();
}

//键盘LED状态指示灯设置(casps/numlock/sclk)
void led_set(uint8_t keyboard_led)
{
    send_rgb_status(SIGNAL_MODE, keyboard_led);
}
//指示灯任务0
void all_state_led_thread(void *arg)
{
    while (1)
    {
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        led_setleds(led_state, LEDSIGNAL_NUM);
        led_counter = 0;
        wait_ms(STATE_LED_TIMERS);
    }
}
#else

uint32_t led_counter = 0;
void led_check(uint8_t id, uint8_t value){
    if(id ==SIGNAL_MODE){
        nrf_gpio_pin_write(NUML_LED, value & (1 << 0) ? 0 : 1);
        nrf_gpio_pin_write(CAPS_LED, value & (1 << 1) ? 0 : 1);
        nrf_gpio_pin_write(SCR_LED, value & (1 << 2) ? 0 : 1);
    }
}

void keyboard_led_init(void)
{
    nrf_gpio_cfg_output(CAPS_LED);
    nrf_gpio_cfg_output(NUML_LED);
    nrf_gpio_cfg_output(SCR_LED);
    nrf_gpio_pin_set(CAPS_LED);
    nrf_gpio_pin_set(NUML_LED);
    nrf_gpio_pin_set(SCR_LED);
}
void keyboard_led_timeout(void){
    // keyboard_led_init();
}
void turn_off_led_device(void)
{
    nrf_gpio_cfg_default(CAPS_LED);
    nrf_gpio_cfg_default(NUML_LED);
    nrf_gpio_cfg_default(SCR_LED);
}

//键盘LED状态指示灯设置(casps/numlock/sclk)
void led_set(uint8_t keyboard_led)
{
    led_counter = 0;
    send_rgb_status(SIGNAL_MODE, keyboard_led);
}

#endif