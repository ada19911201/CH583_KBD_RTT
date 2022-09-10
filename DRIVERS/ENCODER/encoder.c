/*
Copyright (C) 2020 Jim Jiang <jim@lotlab.org>

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


#include "keyboard_event.h"
#if defined(NRF52840_XXAA)
#include "nrf52840_bitfields.h"
#elif defined(NRF52833_XXAA)
#include "nrf52833_bitfields.h"
#endif
#include "nrf_qdec.h"
#include "nrfx_qdec.h"
#include "nrf_gpio.h"
#include "config.h"


const nrfx_qdec_config_t qdec_config = {
    .reportper = NRF_QDEC_REPORTPER_10,
    .sampleper = NRF_QDEC_SAMPLEPER_512us,
    .psela = ROTARY_ENCODER_A,
    .pselb = ROTARY_ENCODER_B,
    .pselled = NRF_QDEC_LED_NOT_CONNECTED, // disable
    .dbfen = true,
    .sample_inten = false,
    .interrupt_priority = APP_IRQ_PRIORITY_MID,
};

static int8_t count;
static int8_t last_count;

void decoder_event_handler(nrfx_qdec_event_t event)
{
#ifndef LOW_POWER_QDEC
    if (event.type == NRF_QDEC_EVENT_REPORTRDY) {
        count += event.data.report.acc;
        if (event.data.report.acc != 0 && count % 4 == 0) {
            if (count > last_count) {
                send_event_message(ENCODER_LEFT);
            } else {
                send_event_message(ENCODER_RIGHT);
            }
            last_count = count;
        }
    }
#else
    if (event.type == NRF_QDEC_EVENT_SAMPLERDY) {
        if (event.data.sample.value == 0 || event.data.sample.value == 2)
            return;

        count += event.data.sample.value;
        if (count % 4 == 0) {
            if (count > last_count) {
                type_code(KC_VOLD);
            } else {
                type_code(KC_VOLU);
            }
            last_count = count;
        }
    }
#endif
}


/**
 * @brief 初始化解码器
 * 
 */
void encoder_init()
{
    nrfx_qdec_init(&qdec_config, decoder_event_handler);
#ifdef LOW_POWER_QDEC
    nrf_qdec_shorts_enable(QDEC_SHORTS_SAMPLERDY_STOP_Msk);
#endif

    nrf_gpio_cfg_input(qdec_config.psela, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(qdec_config.pselb, NRF_GPIO_PIN_PULLUP);

    nrfx_qdec_enable();
}

