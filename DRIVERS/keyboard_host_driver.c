#include "keyboard_host_driver.h"
#include "usbuser.h"
// //定义USB报文类型,0为Boot,1为Report(默认为1,端口支持6为boot模式，切换到nkro用端口2发送数据，端口2不支持boot)
uint8_t keyboard_protocol = 1;

struct device_led usb_ble_led;
//LED状态发送函数
uint8_t keyboard_leds()
{

}
/**
 * @brief 发送按键包
 * 
 * @param index 类型Index。0: generic, 1: mouse, 2: system, 3: consumer, 0x80: nkro
 * @param len 长度
 * @param keys 按键
 */
void send_key_package(uint8_t index, uint8_t len, uint8_t* keys)
{
    usb_send_key(index,len,keys);
}

void send_keyboard(report_keyboard_t* report)
{
#if defined(NKRO_ENABLE)
    if (keyboard_protocol && keyboard_nkro) {
        send_key_package(0x80, NKRO_EPSIZE, report->raw);
    } else
#endif
    {
        send_key_package(0, 8, report->raw);
    }
}
void send_mouse(report_mouse_t* report)
{
    send_key_package(REPORT_ID_MOUSE, sizeof(report_mouse_t), (uint8_t*)report);
}

void send_system(uint16_t data)
{
    send_key_package(REPORT_ID_SYSTEM, 2, (uint8_t*)&data);
}

void send_consumer(uint16_t data)
{
    send_key_package(REPORT_ID_CONSUMER, 2, (uint8_t*)&data);
}

host_driver_t driver = {
    keyboard_leds,
    send_keyboard,
    send_mouse,
    send_system,
    send_consumer,
};
