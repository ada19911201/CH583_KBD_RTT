/*
Copyright 2018 Sekigon   modify by Geno

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

#include "ledsignal.h"
#include "config.h"
#include "nrf_drv_pwm.h"
#include "nrf_log_ctrl.h"
#include "nrfx_power.h"
#include "wait.h"


#define PWM_PERIOD 18
#define PWM_0H_DURATION 5
#define PWM_1H_DURATION 13

nrf_drv_pwm_t pwm1 = NRFX_PWM_INSTANCE(1);

#define DUMMY_SIGNAL_LEN1 10

void pwm_handler1(nrfx_pwm_evt_type_t event_type)
{
}

void led_setleds(LED_TYPE* ledsignal, uint16_t number_leds)
{
    led_setleds_pin(ledsignal, number_leds, LEDSIGNAL_DI_PIN);
}

void led_setleds_pin(LED_TYPE* ledsignal, uint16_t number_leds, uint8_t pinmask1)
{
    static bool flag_init1;
    if (!flag_init1) {
        nrfx_pwm_config_t pwm1_config = {
            .output_pins = {
                NRF_DRV_PWM_PIN_NOT_USED,// channel 0
                pinmask1,// channel 1
                NRF_DRV_PWM_PIN_NOT_USED, // channel 2
                NRF_DRV_PWM_PIN_NOT_USED // channel 3
            },
            .irq_priority = APP_IRQ_PRIORITY_LOW,
            .base_clock = NRF_PWM_CLK_16MHz,
            .count_mode = NRF_PWM_MODE_UP,
            .top_value = PWM_PERIOD,
            .load_mode = NRF_PWM_LOAD_COMMON,
            .step_mode = NRF_PWM_STEP_AUTO
        };
        nrfx_pwm_init(&pwm1, &pwm1_config, pwm_handler1);
        flag_init1 = true;
    }

    const uint16_t t0H = PWM_0H_DURATION | (0x8000);
    const uint16_t t1H = PWM_1H_DURATION | (0x8000);
    nrf_pwm_values_common_t leds[LEDSIGNAL_NUM * 3 * 8 + DUMMY_SIGNAL_LEN1 * 3 * 8 + 8];
    nrf_pwm_values_t p_leds = { .p_common = leds };
    nrf_pwm_sequence_t pwm_seq1 = {
        .values = p_leds,
        .length = LEDSIGNAL_NUM * 3 * 8 + DUMMY_SIGNAL_LEN1 * 3 * 8 + 8,
        .repeats = 0,
        .end_delay = 0,
    };
    uint16_t* p_dat1 = &leds[0];

    for (int i = 0; i < number_leds; i++) {
        uint8_t mask1 = 0x80;
        for (int j = 0; j < 8; j++) {
            *p_dat1++ = (ledsignal[i].g & (mask1 >> j)) ? t1H : t0H;
        }
        mask1 = 0x80;
        for (int j = 0; j < 8; j++) {
            *p_dat1++ = (ledsignal[i].r & (mask1 >> j)) ? t1H : t0H;
        }
        mask1 = 0x80;
        for (int j = 0; j < 8; j++) {
            *p_dat1++ = (ledsignal[i].b & (mask1 >> j)) ? t1H : t0H;
        }
    }

    for (int i = 0; i < DUMMY_SIGNAL_LEN1; i++) {
        for (int j = 0; j < 24; j++) {
            *p_dat1++ = t0H;
        }
    }
    // reset signal
    *p_dat1++ = 0x8000;
    *p_dat1++ = 0x8000;
    *p_dat1++ = 0x8000;
    *p_dat1++ = 0x8000;
    *p_dat1++ = 0x8000;
    *p_dat1++ = 0x8000;
    *p_dat1++ = 0x8000;
    *p_dat1++ = 0x8000;
    nrfx_pwm_simple_playback(&pwm1, &pwm_seq1, 1, NRFX_PWM_FLAG_STOP);
}

void led_setleds_rgbw(LED_TYPE* ledsignal, uint16_t number_leds)
{
    const uint16_t t0H = ((int)(0.35f / 0.0625f)) | (0x8000);
    const uint16_t t1H = ((int)(1.36f / 0.0625f)) | (0x8000);
    nrf_pwm_values_common_t leds[LEDSIGNAL_NUM * 3 * 8 + 1];
    nrf_pwm_values_t p_leds = { .p_common = leds };
    nrf_pwm_sequence_t pwm_seq1 = {
        .values = p_leds,
        .length = sizeof(leds)
            / sizeof(leds[0]),
        .repeats = 0,
        .end_delay = 0,
    };
    uint16_t* p_dat1 = &leds[0];

    for (int i = 0; i < number_leds; i++) {
        uint8_t mask1 = 1;
        for (int j = 0; j < 8; j++) {
            *p_dat1++ = (ledsignal[i].r & (mask1 << j)) ? t1H : t0H;
        }
        mask1 = 1;
        for (int j = 0; j < 8; j++) {
            *p_dat1++ = (ledsignal[i].g & (mask1 << j)) ? t1H : t0H;
        }
        mask1 = 1;
        for (int j = 0; j < 8; j++) {
            *p_dat1++ = (ledsignal[i].b & (mask1 << j)) ? t1H : t0H;
        }
    }
    *p_dat1++ = 0x8000;
    nrfx_pwm_simple_playback(&pwm1, &pwm_seq1, 1, NRFX_PWM_FLAG_STOP);
}
