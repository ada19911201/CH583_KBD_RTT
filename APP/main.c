#include "CH58x_common.h"
#include "task_thread.h"
#include "eeprom.h"
#include <rtthread.h>

int main(void)
{
    rt_enter_critical();
    /* create two task */
#if(defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
#endif
    // Creat_ble_task();
    storage_read(0xff);
    Creat_usb_task();
    Creat_event_task();
    Creat_kbd_task();
#ifdef RGB_TOP_ENABLE
    Creat_rgb_matrix_task();
#endif
    rt_exit_critical();
    return 0;
}

