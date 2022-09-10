/*
Copyright (C) 2018,2019 Jim Jiang <jim@lotlab.org>

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

#include <stdint.h>
#include <rtthread.h>
#include "wait_api.h"
#include "timer.h"
// inline void delay_us(int us)
// {
// #ifdef __GNUC__
// #define __nop() __asm("NOP")
// #endif
// #ifndef MATRIX_SCAN_DELAY_CYCLE
// #define MATRIX_SCAN_DELAY_CYCLE 64
// #endif
//     for (int i = 0; i < us; i++)
//     {
//         for (int j = 0; j < MATRIX_SCAN_DELAY_CYCLE; j++)
//         {
//             __nop(); //64mhz, 64cycle = 1us, delay 0.625us
//         }
//     }
// }
void timer_init()
{
    // do nothing
}

void timer_clear()
{
    // can't do anything
}

inline uint16_t timer_read()
{
    return (uint16_t)(timer_read32() % 0xFFFF);
}

inline uint32_t timer_read32()
{
    uint32_t time;
    // 24bit, 32768 tick per sec.
    // so only 19 bits are valid.

    // but, tmk does not use 32bit timer
    time = rt_tick_get();
    return time;
}

inline uint16_t timer_elapsed(uint16_t last)
{
    return TIMER_DIFF_16(timer_read(), last);
}

inline uint32_t timer_elapsed32(uint32_t last)
{
    uint32_t time = rt_tick_get();
    uint32_t elapsed = time > last ? (time - last) : (time - last + RT_TICK_MAX);
    return elapsed;
}
