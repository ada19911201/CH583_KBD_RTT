/* Copyright 2018 James Laird-Wah
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "rgb_matrix.h"

/* Each driver needs to define the struct
 *    const rgb_matrix_driver_t rgb_matrix_driver;
 * All members must be provided.
 * Keyboard custom drivers can define this in their own files, it should only
 * be here if shared between boards.
 */

#if defined(IS31FL3733) || defined(IS31FL3737) || defined(IS31FL3741)
//如果定义33和37、41,则以下程序执行
#include "i2c_master.h"

static void init(void) {
        //初始化
#ifdef  IS31FL3733
    IS31FL3733_init(DRIVER_ADDR_1, 0);
#elif   IS31FL3737
    IS31FL3737_init(DRIVER_ADDR_1);
#elif   IS31FL3741
    IS31FL3741_init(DRIVER_ADDR_1);
#endif
    for (int index = 0; index < DRIVER_LED_TOTAL; index++) {
        bool enabled = true;
        // This only caches it for later
#ifdef  IS31FL3733
        IS31FL3733_set_led_control_register(index, enabled, enabled, enabled);
#elif   IS31FL3737
        IS31FL3737_set_led_control_register(index, enabled, enabled, enabled);
#elif   IS31FL3741
        IS31FL3741_set_led_control_register(index, enabled, enabled, enabled);      
#endif
    }
    // This actually updates the LED drivers
#ifdef IS31FL3733
    IS31FL3733_update_led_control_registers(DRIVER_ADDR_1, 0);
#elif  IS31FL3737
    IS31FL3737_update_led_control_registers(DRIVER_ADDR_1, 0);
#elif  IS31FL3741
    IS31FL3741_update_led_control_registers(DRIVER_ADDR_1, 0);   
#endif
}

static bool softdown(bool sw) {
        //初始化
#ifdef  IS31FL3733
    return IS31FL3733_softdown(DRIVER_ADDR_1,sw);
#elif   IS31FL3737
    return IS31FL3737_softdown(DRIVER_ADDR_1,sw);
#elif   IS31FL3741
    return IS31FL3741_softdown(DRIVER_ADDR_1,sw);
#endif
}

#ifdef IS31FL3733
static void flush(void) {
    IS31FL3733_update_pwm_buffers(DRIVER_ADDR_1, 0);
}
const rgb_matrix_driver_t rgb_matrix_driver = {
    .init = init,
    .flush = flush,
    .set_color = IS31FL3733_set_color,
    .set_color_all = IS31FL3733_set_color_all,
    .softdown = softdown,
};

#elif IS31FL3737
static void flush(void) {
    IS31FL3737_update_pwm_buffers(DRIVER_ADDR_1, 0);
}

const rgb_matrix_driver_t rgb_matrix_driver = {
    .init = init,
    .flush = flush,
    .set_color = IS31FL3737_set_color,
    .set_color_all = IS31FL3737_set_color_all,
    .softdown = softdown,
};

#elif IS31FL3741
static void flush(void) {
    IS31FL3741_update_pwm_buffers(DRIVER_ADDR_1, 0);
}

const rgb_matrix_driver_t rgb_matrix_driver = {
    .init = init,
    .flush = flush,
    .set_color = IS31FL3741_set_color,
    .set_color_all = IS31FL3741_set_color_all,
    .softdown = softdown,
};
#endif

#endif