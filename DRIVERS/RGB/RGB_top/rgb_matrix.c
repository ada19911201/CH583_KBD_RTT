/* Copyright 2017 Jason Williams
 * Copyright 2017 Jack Humbert
 * Copyright 2018 Yiancar
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
#include "progmem.h"
#include "config.h"
#include <string.h>
#include <math.h>
#include "timer.h"
#include "wait_api.h"
#include "lib8tion/lib8tion.h"
#include "eeconfig.h"
#include "i2c_master.h"
//包含储存区头文件
#include "eeprom.h"
//引入事件头文件
#include "keyboard_event.h"
//
#include "task_thread.h"
//
#include "xprintf.h"
#ifndef RGB_MATRIX_CENTER
const point_t k_rgb_matrix_center = {112, 32};
#else
const point_t k_rgb_matrix_center = RGB_MATRIX_CENTER;
#endif

// Generic effect runners
#include "rgb_matrix_runners/effect_runner_dx_dy_dist.h"
#include "rgb_matrix_runners/effect_runner_dx_dy.h"
#include "rgb_matrix_runners/effect_runner_i.h"
#include "rgb_matrix_runners/effect_runner_sin_cos_i.h"
#include "rgb_matrix_runners/effect_runner_reactive.h"
#include "rgb_matrix_runners/effect_runner_reactive_splash.h"

// ------------------------------------------
// -----Begin rgb effect includes macros-----
#define RGB_MATRIX_EFFECT(name)
#define RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include "rgb_matrix_animations/rgb_matrix_effects.inc"
#ifdef RGB_MATRIX_CUSTOM_KB
#    include "rgb_matrix_kb.inc"
#endif
#ifdef RGB_MATRIX_CUSTOM_USER
#    include "rgb_matrix_user.inc"
#endif

#undef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#undef RGB_MATRIX_EFFECT
// -----End rgb effect includes macros-------
// ------------------------------------------

#if defined(RGB_DISABLE_AFTER_TIMEOUT) && !defined(RGB_DISABLE_TIMEOUT)
#    define RGB_DISABLE_TIMEOUT (RGB_DISABLE_AFTER_TIMEOUT * 1200)
#endif

#ifndef RGB_DISABLE_TIMEOUT
#    define RGB_DISABLE_TIMEOUT 0
#endif

#ifndef RGB_DISABLE_WHEN_USB_SUSPENDED
#    define RGB_DISABLE_WHEN_USB_SUSPENDED false
#endif

#if !defined(RGB_MATRIX_MAXIMUM_BRIGHTNESS) || RGB_MATRIX_MAXIMUM_BRIGHTNESS > UINT8_MAX
#    undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#    define RGB_MATRIX_MAXIMUM_BRIGHTNESS UINT8_MAX
#endif

#if !defined(RGB_MATRIX_HUE_STEP)
#    define RGB_MATRIX_HUE_STEP 8
#endif

#if !defined(RGB_MATRIX_SAT_STEP)
#    define RGB_MATRIX_SAT_STEP 16
#endif

#if !defined(RGB_MATRIX_VAL_STEP)
#    define RGB_MATRIX_VAL_STEP 16
#endif

#if !defined(RGB_MATRIX_SPD_STEP)
#    define RGB_MATRIX_SPD_STEP 16
#endif

#if !defined(RGB_MATRIX_STARTUP_MODE)
#    ifndef DISABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#        define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_CYCLE_LEFT_RIGHT
#    else
// fallback to solid colors if RGB_MATRIX_CYCLE_LEFT_RIGHT is disabled in userspace
#        define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_SOLID_COLOR
#    endif
#endif

#if !defined(RGB_MATRIX_STARTUP_HUE)
#    define RGB_MATRIX_STARTUP_HUE 0
#endif

#if !defined(RGB_MATRIX_STARTUP_SAT)
#    define RGB_MATRIX_STARTUP_SAT UINT8_MAX
#endif

#if !defined(RGB_MATRIX_STARTUP_VAL)
#    define RGB_MATRIX_STARTUP_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS
#endif

#if !defined(RGB_MATRIX_STARTUP_SPD)
#    define RGB_MATRIX_STARTUP_SPD UINT8_MAX / 2
#endif

// globals
bool         g_suspend_state = false;
rgb_config_t rgb_matrix_config;  // TODO: would like to prefix this with g_ for global consistancy, do this in another pr
uint32_t     g_rgb_timer;
#ifdef RGB_MATRIX_FRAMEBUFFER_EFFECTS
uint8_t g_rgb_frame_buffer[MATRIX_ROWS][MATRIX_COLS] = {{0}};
#endif  // RGB_MATRIX_FRAMEBUFFER_EFFECTS
#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
last_hit_t g_last_hit_tracker;
#endif  // RGB_MATRIX_KEYREACTIVE_ENABLED

// internals
static uint8_t         rgb_last_enable   = UINT8_MAX;
static uint8_t         rgb_last_effect   = UINT8_MAX;
static effect_params_t rgb_effect_params = {0, 0xFF};
static rgb_task_states rgb_task_state    = SYNCING;
#if RGB_DISABLE_TIMEOUT > 0
static uint32_t rgb_anykey_timer;
#endif  // RGB_DISABLE_TIMEOUT > 0

// double buffers
static uint32_t rgb_timer_buffer;
#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
static last_hit_t last_hit_buffer;
#endif  // RGB_MATRIX_KEYREACTIVE_ENABLED

//读取matrix_rgb存储数据
static void eeprom_read_block(void *buf, uint32_t len) {
    for (uint8_t i = 0; i < len; i++)
    {
        *(uint8_t *)buf++ = config_block[i+RGB_MATRIX_CONFIG_P];
    }
}
//写入matrix_rgb存储数据
static void eeprom_update_block(void *buf, uint32_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        config_block[i+RGB_MATRIX_CONFIG_P] = *(uint8_t *)buf++;
    }
    storage_write((1 << STORAGE_CONFIG));
}

void eeconfig_read_rgb_matrix(void) { eeprom_read_block(&rgb_matrix_config, RGB_MATRIX_CONFIG_SIZE); }

void eeconfig_update_rgb_matrix(void) { eeprom_update_block(&rgb_matrix_config, RGB_MATRIX_CONFIG_SIZE); }

void eeconfig_update_rgb_matrix_default(void) {
    // dprintf("eeconfig_update_rgb_matrix_default\n");
    rgb_matrix_config.enable = RGB_MATRIX_STARTUP_ENABLE;
    rgb_matrix_config.mode   = RGB_MATRIX_STARTUP_MODE;
    rgb_matrix_config.hsv    = (HSV){RGB_MATRIX_STARTUP_HUE, RGB_MATRIX_STARTUP_SAT, RGB_MATRIX_STARTUP_VAL};
    rgb_matrix_config.speed  = RGB_MATRIX_STARTUP_SPD;
    eeconfig_update_rgb_matrix();
}

bool rgb_ic_softdown(bool sw){
    return rgb_matrix_driver.softdown(sw);
}
static void eeconfig_debug_rgb_matrix(void) {
    // NRF_LOG_INFO("rgb_matrix_config eprom\n");
    // xprintf("rgb_matrix_config.enable = %d\n", rgb_matrix_config.enable);
    // NRF_LOG_INFO("rgb_matrix_config.mode = %d\n", rgb_matrix_config.mode);
    // NRF_LOG_INFO("rgb_matrix_config.hsv.h = %d\n", rgb_matrix_config.hsv.h);
    // NRF_LOG_INFO("rgb_matrix_config.hsv.s = %d\n", rgb_matrix_config.hsv.s);
    // NRF_LOG_INFO("rgb_matrix_config.hsv.v = %d\n", rgb_matrix_config.hsv.v);
    // NRF_LOG_INFO("rgb_matrix_config.speed = %d\n", rgb_matrix_config.speed);
}

__attribute__((weak)) uint8_t rgb_matrix_map_row_column_to_led_kb(uint8_t row, uint8_t column, uint8_t *led_i) { return 0; }

uint8_t rgb_matrix_map_row_column_to_led(uint8_t row, uint8_t column, uint8_t *led_i) {
    uint8_t led_count = rgb_matrix_map_row_column_to_led_kb(row, column, led_i);
    uint8_t led_index = g_led_config.matrix_co[row][column];
    if (led_index != NO_LED) {
        led_i[led_count] = led_index;
        led_count++;
    }
    return led_count;
}

void rgb_matrix_update_pwm_buffers(void) { rgb_matrix_driver.flush(); }

void rgb_matrix_set_color(int index, uint8_t red, uint8_t green, uint8_t blue) { rgb_matrix_driver.set_color(index, red, green, blue); }

void rgb_matrix_set_color_all(uint8_t red, uint8_t green, uint8_t blue) { rgb_matrix_driver.set_color_all(red, green, blue); }

void process_rgb_matrix(uint8_t row, uint8_t col, bool pressed) {
#if RGB_DISABLE_TIMEOUT > 0
    rgb_anykey_timer = 0;
#endif  // RGB_DISABLE_TIMEOUT > 0

#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
    uint8_t led[LED_HITS_TO_REMEMBER];
    uint8_t led_count = 0;

#    if defined(RGB_MATRIX_KEYRELEASES)
    if (!pressed)
#    elif defined(RGB_MATRIX_KEYPRESSES)
    if (pressed)
#    endif  // defined(RGB_MATRIX_KEYRELEASES)
    {
        led_count = rgb_matrix_map_row_column_to_led(row, col, led);
    }

    if (last_hit_buffer.count + led_count > LED_HITS_TO_REMEMBER) {
        memcpy(&last_hit_buffer.x[0], &last_hit_buffer.x[led_count], LED_HITS_TO_REMEMBER - led_count);
        memcpy(&last_hit_buffer.y[0], &last_hit_buffer.y[led_count], LED_HITS_TO_REMEMBER - led_count);
        memcpy(&last_hit_buffer.tick[0], &last_hit_buffer.tick[led_count], (LED_HITS_TO_REMEMBER - led_count) * 2);  // 16 bit
        memcpy(&last_hit_buffer.index[0], &last_hit_buffer.index[led_count], LED_HITS_TO_REMEMBER - led_count);
        last_hit_buffer.count = LED_HITS_TO_REMEMBER - led_count;
    }

    for (uint8_t i = 0; i < led_count; i++) {
        uint8_t index                = last_hit_buffer.count;
        last_hit_buffer.x[index]     = g_led_config.point[led[i]].x;
        last_hit_buffer.y[index]     = g_led_config.point[led[i]].y;
        last_hit_buffer.index[index] = led[i];
        last_hit_buffer.tick[index]  = 0;
        last_hit_buffer.count++;
    }
#endif  // RGB_MATRIX_KEYREACTIVE_ENABLED

#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP)
    if (rgb_matrix_config.mode == RGB_MATRIX_TYPING_HEATMAP) {
        process_rgb_matrix_typing_heatmap(row, col);
    }
#endif  // defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP)
}

void rgb_matrix_test(void) {
    // Mask out bits 4 and 5
    // Increase the factor to make the test animation slower (and reduce to make it faster)
    uint8_t factor = 10;
    switch ((g_rgb_timer & (0b11 << factor)) >> factor) {
        case 0: {
            rgb_matrix_set_color_all(20, 0, 0);
            break;
        }
        case 1: {
            rgb_matrix_set_color_all(0, 20, 0);
            break;
        }
        case 2: {
            rgb_matrix_set_color_all(0, 0, 20);
            break;
        }
        case 3: {
            rgb_matrix_set_color_all(20, 20, 20);
            break;
        }
    }
}

static bool rgb_matrix_none(effect_params_t *params) {
    if (!params->init) {
        return false;
    }

    rgb_matrix_set_color_all(0, 0, 0);
    return false;
}

static void rgb_task_timers(void) {
#if defined(RGB_MATRIX_KEYREACTIVE_ENABLED) || RGB_DISABLE_TIMEOUT > 0
    uint32_t deltaTime = timer_elapsed32(rgb_timer_buffer);
#endif  // defined(RGB_MATRIX_KEYREACTIVE_ENABLED) || RGB_DISABLE_TIMEOUT > 0
    rgb_timer_buffer = timer_read32();

    // Update double buffer timers
#if RGB_DISABLE_TIMEOUT > 0
    if (rgb_anykey_timer < UINT32_MAX) {
        if (UINT32_MAX - deltaTime < rgb_anykey_timer) {
            rgb_anykey_timer = UINT32_MAX;
        } else {
            rgb_anykey_timer += deltaTime;
        }
    }
#endif  // RGB_DISABLE_TIMEOUT > 0

    // Update double buffer last hit timers
#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
    uint8_t count = last_hit_buffer.count;
    for (uint8_t i = 0; i < count; ++i) {
        if (UINT16_MAX - deltaTime < last_hit_buffer.tick[i]) {
            last_hit_buffer.count--;
            continue;
        }
        last_hit_buffer.tick[i] += deltaTime;
    }
#endif  // RGB_MATRIX_KEYREACTIVE_ENABLED
}

static void rgb_task_sync(void) {
    // next task
    if (timer_elapsed32(g_rgb_timer) >= RGB_MATRIX_LED_FLUSH_LIMIT) rgb_task_state = STARTING;
}

static void rgb_task_start(void) {
    // reset iter
    rgb_effect_params.iter = 0;

    // update double buffers
    g_rgb_timer = rgb_timer_buffer;
#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
    g_last_hit_tracker = last_hit_buffer;
#endif  // RGB_MATRIX_KEYREACTIVE_ENABLED

    // next task
    rgb_task_state = RENDERING;
}

static void rgb_task_render(uint8_t effect) {
    bool rendering         = false;
    rgb_effect_params.init = (effect != rgb_last_effect) || (rgb_matrix_config.enable != rgb_last_enable);

    // each effect can opt to do calculations
    // and/or request PWM buffer updates.
    switch (effect) {
        case RGB_MATRIX_NONE:
            rendering = rgb_matrix_none(&rgb_effect_params);
            break;

// ---------------------------------------------
// -----Begin rgb effect switch case macros-----
#define RGB_MATRIX_EFFECT(name, ...)          \
    case RGB_MATRIX_##name:                   \
        rendering = name(&rgb_effect_params); \
        break;
#include "rgb_matrix_animations/rgb_matrix_effects.inc"
#undef RGB_MATRIX_EFFECT

#if defined(RGB_MATRIX_CUSTOM_KB) || defined(RGB_MATRIX_CUSTOM_USER)
#    define RGB_MATRIX_EFFECT(name, ...)          \
        case RGB_MATRIX_CUSTOM_##name:            \
            rendering = name(&rgb_effect_params); \
            break;
#    ifdef RGB_MATRIX_CUSTOM_KB
#        include "rgb_matrix_kb.inc"
#    endif
#    ifdef RGB_MATRIX_CUSTOM_USER
#        include "rgb_matrix_user.inc"
#    endif
#    undef RGB_MATRIX_EFFECT
#endif
            // -----End rgb effect switch case macros-------
            // ---------------------------------------------

        // Factory default magic value
        case UINT8_MAX: {
            rgb_matrix_test();
            rgb_task_state = FLUSHING;
        }
            return;
    }

    rgb_effect_params.iter++;

    // next task
    if (!rendering) {
        rgb_task_state = FLUSHING;
        if (!rgb_effect_params.init && effect == RGB_MATRIX_NONE) {
            // We only need to flush once if we are RGB_MATRIX_NONE
            rgb_task_state = SYNCING;
        }
    }
}

static void rgb_task_flush(uint8_t effect) {
    // update last trackers after the first full render so we can init over several frames
    rgb_last_effect = effect;
    rgb_last_enable = rgb_matrix_config.enable;

    // update pwm buffers
    rgb_matrix_update_pwm_buffers();

    // next task
    rgb_task_state = SYNCING;
}

void rgb_matrix_task(void *arg)
{
    i2c_init();
    rgb_matrix_init();
    //不亮
    rgb_matrix_set_suspend_state(true);
    while (1)
    {
        rgb_task_timers();
        // Ideally we would also stop sending zeros to the LED driver PWM buffers
        // while suspended and just do a software shutdown. This is a cheap hack for now.
        bool suspend_backlight =
#if RGB_DISABLE_WHEN_USB_SUSPENDED == true
            g_suspend_state ||
#endif // RGB_DISABLE_WHEN_USB_SUSPENDED == true
#if RGB_DISABLE_TIMEOUT > 0
            (rgb_anykey_timer > (uint32_t)RGB_DISABLE_TIMEOUT) ||
#endif // RGB_DISABLE_TIMEOUT > 0
            false;
        uint8_t effect = suspend_backlight || !rgb_matrix_config.enable ? 0 : rgb_matrix_config.mode;
        switch (rgb_task_state)
        {
        case STARTING:
            rgb_task_start();
            break;
        case RENDERING:
            rgb_task_render(effect);
            if (effect) {
                rgb_matrix_indicators();
                // rgb_matrix_indicators_advanced(&rgb_effect_params);
            }
            break;
        case FLUSHING:
            rgb_task_flush(effect);
            break;
        case SYNCING:
            rgb_task_sync();
            break;
        }
    }
}

void rgb_matrix_indicators(void) {
    rgb_matrix_indicators_kb();
    rgb_matrix_indicators_user();
}

__attribute__((weak)) void rgb_matrix_indicators_kb(void) {}

__attribute__((weak)) void rgb_matrix_indicators_user(void) {}

void rgb_matrix_init(void) {
    rgb_matrix_driver.init();

#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
    g_last_hit_tracker.count = 0;
    for (uint8_t i = 0; i < LED_HITS_TO_REMEMBER; ++i) {
        g_last_hit_tracker.tick[i] = UINT16_MAX;
    }

    last_hit_buffer.count = 0;
    for (uint8_t i = 0; i < LED_HITS_TO_REMEMBER; ++i) {
        last_hit_buffer.tick[i] = UINT16_MAX;
    }
#endif  // RGB_MATRIX_KEYREACTIVE_ENABLED

    // if (!eeconfig_is_enabled()) {
    //     eeconfig_init();
    //     eeconfig_update_rgb_matrix_default();
    // }

    eeconfig_read_rgb_matrix();
    if (!rgb_matrix_config.mode) {
        eeconfig_update_rgb_matrix_default();
    }
    // if (rgb_matrix_config.enable) {
    //     i2c_start();
    // }
    eeconfig_debug_rgb_matrix();  // display current eeprom values
}

void rgb_matrix_set_suspend_state(bool state) {
    if (RGB_DISABLE_WHEN_USB_SUSPENDED && state) {
        rgb_matrix_set_color_all(0, 0, 0);  // turn off all LEDs when suspending
    }
    g_suspend_state = state;
}

bool rgb_matrix_get_suspend_state(void) { return g_suspend_state; }

void rgb_matrix_toggle(void) {
    if (rgb_matrix_config.enable) {
        rgb_matrix_disable();
    } else {
        rgb_matrix_enable();
    }
}

void rgb_matrix_enable(void) {
    rgb_matrix_enable_noeeprom();
    eeconfig_update_rgb_matrix();
}

void rgb_matrix_enable_noeeprom(void) {
    if (!rgb_matrix_config.enable) rgb_task_state = STARTING;
    rgb_matrix_config.enable = 1;
}

void rgb_matrix_disable(void) {
    rgb_matrix_disable_noeeprom();
    eeconfig_update_rgb_matrix();
}

void rgb_matrix_disable_noeeprom(void) {
    if (rgb_matrix_config.enable) rgb_task_state = STARTING;
    rgb_matrix_config.enable = 0;
}

uint8_t rgb_matrix_is_enabled(void) { return rgb_matrix_config.enable; }

void rgb_matrix_step(void) {
    rgb_matrix_config.mode++;
    if (rgb_matrix_config.mode >= RGB_MATRIX_EFFECT_MAX) rgb_matrix_config.mode = 1;
    rgb_task_state = STARTING;
    eeconfig_update_rgb_matrix();
}

void rgb_matrix_step_reverse(void) {
    rgb_matrix_config.mode--;
    if (rgb_matrix_config.mode < 1) rgb_matrix_config.mode = RGB_MATRIX_EFFECT_MAX - 1;
    rgb_task_state = STARTING;
    eeconfig_update_rgb_matrix();
}

void rgb_matrix_increase_hue(void) {
    rgb_matrix_config.hsv.h += RGB_MATRIX_HUE_STEP;
    eeconfig_update_rgb_matrix();
}

void rgb_matrix_decrease_hue(void) {
    rgb_matrix_config.hsv.h -= RGB_MATRIX_HUE_STEP;
    eeconfig_update_rgb_matrix();
}

void rgb_matrix_increase_sat(void) {
    rgb_matrix_config.hsv.s = qadd8(rgb_matrix_config.hsv.s, RGB_MATRIX_SAT_STEP);
    eeconfig_update_rgb_matrix();
}

void rgb_matrix_decrease_sat(void) {
    rgb_matrix_config.hsv.s = qsub8(rgb_matrix_config.hsv.s, RGB_MATRIX_SAT_STEP);
    eeconfig_update_rgb_matrix();
}

void rgb_matrix_increase_val(void) {
    rgb_matrix_config.hsv.v = qadd8(rgb_matrix_config.hsv.v, RGB_MATRIX_VAL_STEP);
    if (rgb_matrix_config.hsv.v > RGB_MATRIX_MAXIMUM_BRIGHTNESS) rgb_matrix_config.hsv.v = RGB_MATRIX_MAXIMUM_BRIGHTNESS;
    eeconfig_update_rgb_matrix();
}

void rgb_matrix_decrease_val(void) {
    rgb_matrix_config.hsv.v = qsub8(rgb_matrix_config.hsv.v, RGB_MATRIX_VAL_STEP);
    eeconfig_update_rgb_matrix();
}

void rgb_matrix_increase_speed(void) {
    rgb_matrix_config.speed = qadd8(rgb_matrix_config.speed, RGB_MATRIX_SPD_STEP);
    eeconfig_update_rgb_matrix();
}

void rgb_matrix_decrease_speed(void) {
    rgb_matrix_config.speed = qsub8(rgb_matrix_config.speed, RGB_MATRIX_SPD_STEP);
    eeconfig_update_rgb_matrix();
}

uint8_t rgb_matrix_get_speed(void) { return rgb_matrix_config.speed; }

led_flags_t rgb_matrix_get_flags(void) { return rgb_effect_params.flags; }

void rgb_matrix_set_flags(led_flags_t flags) { rgb_effect_params.flags = flags; }

void rgb_matrix_mode(uint8_t mode) {
    rgb_matrix_config.mode = mode;
    rgb_task_state         = STARTING;
    eeconfig_update_rgb_matrix();
}

void rgb_matrix_mode_noeeprom(uint8_t mode) { rgb_matrix_config.mode = mode; }

uint8_t rgb_matrix_get_mode(void) { return rgb_matrix_config.mode; }

void rgb_matrix_sethsv(uint16_t hue, uint8_t sat, uint8_t val) {
    rgb_matrix_sethsv_noeeprom(hue, sat, val);
    eeconfig_update_rgb_matrix();
}

void rgb_matrix_sethsv_noeeprom(uint16_t hue, uint8_t sat, uint8_t val) {
    rgb_matrix_config.hsv.h = hue;
    rgb_matrix_config.hsv.s = sat;
    rgb_matrix_config.hsv.v = val;
    if (rgb_matrix_config.hsv.v > RGB_MATRIX_MAXIMUM_BRIGHTNESS) rgb_matrix_config.hsv.v = RGB_MATRIX_MAXIMUM_BRIGHTNESS;
}

HSV     rgb_matrix_get_hsv(void) { return rgb_matrix_config.hsv; }
uint8_t rgb_matrix_get_hue(void) { return rgb_matrix_config.hsv.h; }
uint8_t rgb_matrix_get_sat(void) { return rgb_matrix_config.hsv.s; }
uint8_t rgb_matrix_get_val(void) { return rgb_matrix_config.hsv.v; }


