/*
Copyright 2013 Jun Wako <wakojun@gmail.com>

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
#include "action.h"
#include "action_util.h"
#include "action_macro.h"
#include "wait.h"
#include "timer.h"
#ifdef DEBUG_ACTION
#include "debug.h"
#else
#include "nodebug.h"
#endif

#ifndef NO_ACTION_MACRO
#define DELAY_TIME 7
#define MACRO_READ() (macro = MACRO_GET(macro_p++))
void action_macro_play(const macro_t *macro_p)
{
    macro_t macro = END;

    uint8_t mod_storage = 0;

    if (!macro_p)
        return;
    while (true)
    {
        switch (MACRO_READ())
        {
        case KEY_DOWN:
            MACRO_READ();
            // NRF_LOG_INFO("KEY_DOWN(%02X)\n", macro);
            if (IS_MOD(macro))
            {
                add_weak_mods(MOD_BIT(macro));
                send_keyboard_report();
            }
            else
            {
                register_code(macro);
            }
            wait_ms(DELAY_TIME);
            break;
        case KEY_UP:
            MACRO_READ();
            // NRF_LOG_INFO("KEY_UP(%02X)\n", macro);
            if (IS_MOD(macro))
            {
                del_weak_mods(MOD_BIT(macro));
                send_keyboard_report();
            }
            else
            {
                unregister_code(macro);
            }
            wait_ms(DELAY_TIME);
            break;
        case WAIT:
            MACRO_READ();
            wait_ms(macro);
            break;
        case INTERVAL:
            wait_ms(MACRO_READ());
            break;
        case MOD_STORE:
            mod_storage = get_mods();
            break;
        case MOD_RESTORE:
            set_mods(mod_storage);
            send_keyboard_report();
            wait_ms(DELAY_TIME);
            break;
        case MOD_CLEAR:
            clear_mods();
            send_keyboard_report();
            wait_ms(DELAY_TIME);
            break;
        case 0x04 ... 0x73:
            register_code(macro);
            wait_ms(DELAY_TIME);
            break;
        case 0x84 ... 0xF3:
            unregister_code(macro & 0x7F);
            wait_ms(DELAY_TIME);
            break;
        case END:
        default:
            return;
        }
    }
}

#define TIMER_INTERVAL 2000
uint8_t action_tapmacro_play(const macro_t *macro_p, uint8_t number)
{
    static uint32_t timers_count = 0;
    macro_t macro = END;
    uint8_t count_number = 2;
    if (number == 0)
    {
        timers_count = timer_read32();
    }
    else
    {
        if (TIMER_INTERVAL > timer_elapsed32(timers_count))
        {
            timers_count = timer_read32();
        }
        else
        {
            timers_count = timer_read32();
            number = 0;
        }
    }
    // NRF_LOG_INFO("TIMER_INTERVAL");

    if (!macro_p)
        return 0;
    while (count_number--)
    {
        switch (*(macro_p + number++))
        {
        case KEY_DOWN:
            macro = *(macro_p + number++);
            if (IS_MOD(macro))
            {
                add_weak_mods(MOD_BIT(macro));
                send_keyboard_report();
            }
            else
            {
                register_code(macro);
            }
            break;
        case KEY_UP:
            macro = *(macro_p + number++);
            if (IS_MOD(macro))
            {
                del_weak_mods(MOD_BIT(macro));
                send_keyboard_report();
            }
            else
            {
                unregister_code(macro);
            }
            break;
        case END:
            return 0;
        default:
            return 0;
        }
        wait_ms(DELAY_TIME);
    }
    if (*(macro_p + number++) == END)
    {
        return 0;
    }
    // NRF_LOG_INFO("number = %d", number - 1);
    return number - 1;
}

#endif
