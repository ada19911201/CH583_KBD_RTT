#pragma once

#include <rtthread.h>

#define wait_ms(ms) rt_thread_delay(ms)

void delay_us(int us);

