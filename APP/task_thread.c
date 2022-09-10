#include "task_thread.h"
#include "usbuser.h"
#include "keyboard.h"
#include "wait_api.h"
#include "matrix.h"
#ifdef RGB_TOP_ENABLE
#include "i2c_master.h"
#endif
#include "keyboard_host_driver.h"
#include "xprintf.h"

/* 创建一个邮箱 */
static rt_mailbox_t kbd_event_mail = RT_NULL;

static void Creat_event_mail(void){
    kbd_event_mail = rt_mb_create("kbd_event_mail", /* 邮箱名字 */
                             5,//邮箱大小
                             RT_IPC_FLAG_FIFO);/* 信号量模式 FIFO(0x00)*/
}

static rt_thread_t kbd_task_thread = RT_NULL;

static void kbd_task_entry(void *pvParameters)
{
    matrix_init();
    host_set_driver(&driver); // 设置 host driver
    while(1)
    {
        keyboard_task();
        wait_ms(10);
    }
}

void Creat_kbd_task(void){

    kbd_task_thread =  rt_thread_create( "kbd_task", /* 线程名字 */
                        kbd_task_entry, /* 线程入口函数 */
                        RT_NULL, /* 线程入口函数参数 */
                        512, /* 线程栈大小 */
                        2, /* 线程的优先级 */
                        5); /* 线程时间片 */

    if (kbd_task_thread != RT_NULL)
        rt_thread_startup(kbd_task_thread);
}


static rt_thread_t kbd_event_thread = RT_NULL;
static void kbd_event_entry(void *pvParameters)
{
    rt_err_t uwRet = RT_EOK;
    char *r_str;
    while(1){
    /* 等待接邮箱消息 */
     uwRet = rt_mb_recv(kbd_event_mail, /* 邮箱对象句柄 */
                        (rt_ubase_t*)&r_str, /* 接收邮箱消息 */
                        RT_WAITING_FOREVER); /* 指定超时事件, 一直等 */

     if (RT_EOK == uwRet) { /* 如果接收完成并且正确 */
        }
    }
}

void Creat_event_task(void){
    Creat_event_mail();

    kbd_event_thread =  rt_thread_create( "kbd_event", /* 线程名字 */
                        kbd_event_entry, /* 线程入口函数 */
                        RT_NULL, /* 线程入口函数参数 */
                        256, /* 线程栈大小 */
                        2, /* 线程的优先级 */
                        1); /* 线程时间片 */

    if (kbd_event_thread != RT_NULL)
        rt_thread_startup(kbd_event_thread);
}
#ifdef RGB_TOP_ENABLE
//创建RGB top
static rt_thread_t rgb_matrix_thread = RT_NULL;
void Creat_rgb_matrix_task(void){

    rgb_matrix_thread = rt_thread_create( "rgb_matrix", /* 线程名字 */
                        rgb_matrix_task, /* 线程入口函数 */
                        RT_NULL, /* 线程入口函数参数 */
                        256, /* 线程栈大小 */ //128可以//
                        4, /* 线程的优先级 */
                        10); /* 线程时间片 */

    if (rgb_matrix_thread != RT_NULL)
        rt_thread_startup(rgb_matrix_thread);
}
#endif

#ifdef RGB_BOTTOM_ENABLE
//创建RGB top
static rt_thread_t rgb_light_thread = RT_NULL;
void Creat_rgb_light_task(void){

    rgb_light_thread = rt_thread_create( "rgb_light", /* 线程名字 */
                        rgb_light_task, /* 线程入口函数 */
                        RT_NULL, /* 线程入口函数参数 */
                        256, /* 线程栈大小 */ //128可以//
                        4, /* 线程的优先级 */
                        10); /* 线程时间片 */

    if (rgb_light_thread != RT_NULL)
        rt_thread_startup(rgb_light_thread);
}
#endif
void Creat_usb_task(void){
    USB_Init();
}

// __attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];

// #if(defined(BLE_MAC)) && (BLE_MAC == TRUE)
// const uint8_t MacAddr[6] = {0x84, 0xC2, 0xE4, 0x03, 0x02, 0x02};
// #endif

// static void ble_task_entry(void *pvParameters)
// {
//     CH58X_BLEInit();
//     HAL_Init();
//     GAPRole_PeripheralInit();
//     HidDev_Init();
//     HidEmu_Init();
//     while(1)
//     {
//         TMOS_SystemProcess();
//     }
// }
// //创建ble
// static rt_thread_t ble_thread = RT_NULL;
// void Creat_ble_task(void){

//     ble_thread = rt_thread_create( "ble_task", /* 线程名字 */
//                         ble_task_entry, /* 线程入口函数 */
//                         RT_NULL, /* 线程入口函数参数 */
//                         512, /* 线程栈大小 */ //128可以//
//                         1, /* 线程的优先级 */
//                         1); /* 线程时间片 */

//     if (ble_thread != RT_NULL)
//         rt_thread_startup(ble_thread);
// }