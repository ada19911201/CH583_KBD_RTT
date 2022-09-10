#ifndef TASK_THREAD_H
#define TASK_THREAD_H

#include <rtthread.h>
#include "rtdef.h"
#ifdef RGB_TOP_ENABLE
#include "rgb_matrix.h"
#endif
#ifdef RGB_BOTTOM_ENABLE
#include "rgblight.h"
#endif

void Creat_usb_task(void);
void Creat_kbd_task(void);
void Creat_event_task(void);
// void Creat_ble_task(void);
#ifdef RGB_TOP_ENABLE
void Creat_rgb_matrix_task(void);
#endif
#ifdef RGB_BOTTOM_ENABLE
void Creat_rgb_light_task(void);
#endif
#endif