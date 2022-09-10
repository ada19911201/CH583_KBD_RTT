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

#include "ws2812.h"
#include "config.h"
#include "CH58x_common.h"
#include "wait.h"


#define t0H 32
#define t1H 68


#define DUMMY_SIGNAL_LEN 10

__attribute__((aligned(4))) uint16_t pwm_led[RGBLED_NUM * 3 * 8 + DUMMY_SIGNAL_LEN * 3 * 8 + 8];


void ws2812_setleds(LED_TYPE* ledarray, uint16_t number_of_leds)
{
    ws2812_setleds_pin(ledarray, number_of_leds, RGB_DI_PIN);
}

void ws2812_setleds_pin(LED_TYPE* ledarray, uint16_t number_of_leds, uint8_t pinmask)
{
    // GPIOB_ModeCfg(GPIO_Pin_22, GPIO_ModeOut_PP_5mA);
    // GPIOPinRemap(ENABLE, RB_PIN_TMR2);

    // TMR2_PWMCycleCfg(100); // 周期 1.25us
    // uint16_t* p_dat = &pwm_led[0];

    // for (int i = 0; i < number_of_leds; i++) {
    //     uint8_t mask = 0x80;
    //     for (int j = 0; j < 8; j++) {
    //         *p_dat++ = (ledarray[i].g & (mask >> j)) ? t1H : t0H;
    //     }
    //     mask = 0x80;
    //     for (int j = 0; j < 8; j++) {
    //         *p_dat++ = (ledarray[i].r & (mask >> j)) ? t1H : t0H;
    //     }
    //     mask = 0x80;
    //     for (int j = 0; j < 8; j++) {
    //         *p_dat++ = (ledarray[i].b & (mask >> j)) ? t1H : t0H;
    //     }
    // }

    // for (int i = 0; i < DUMMY_SIGNAL_LEN; i++) {
    //     for (int j = 0; j < 24; j++) {
    //         *p_dat++ = t0H;
    //     }
    // }
    // // reset signal
    // *p_dat++ = 0x8000;
    // *p_dat++ = 0x8000;
    // *p_dat++ = 0x8000;
    // *p_dat++ = 0x8000;
    // *p_dat++ = 0x8000;
    // *p_dat++ = 0x8000;
    // *p_dat++ = 0x8000;
    // *p_dat++ = 0x8000;
    // TMR2_DMACfg(ENABLE, (uint16_t)(uint32_t)&pwm_led[0], (uint16_t)(uint32_t)&pwm_led[RGBLED_NUM * 3 * 8 + DUMMY_SIGNAL_LEN * 3 * 8 + 8], Mode_LOOP);
    // TMR2_PWMInit(Low_Level, PWM_Times_16);
    // /* 开启计数溢出中断，计慢1000个周期进入中断 */
    // TMR2_ClearITFlag(TMR1_2_IT_DMA_END);
    // PFIC_EnableIRQ(TMR2_IRQn);
    // TMR2_ITCfg(ENABLE, TMR1_2_IT_DMA_END);
}

__INTERRUPT
__HIGH_CODE
void TMR2_IRQHandler(void)
{
    if(TMR2_GetITFlag(TMR0_3_IT_CYC_END))
    {
        TMR2_ClearITFlag(TMR0_3_IT_CYC_END);
        /* 计数器计满，硬件自动清零，重新开始计数 */
        /* 用户可自行添加需要的处理 */
    }
    if(TMR2_GetITFlag(TMR1_2_IT_DMA_END))
    {
        TMR2_ClearITFlag(TMR1_2_IT_DMA_END);
        // PRINT("DMA end\n");
        /* DMA 结束 */
        /* 用户可自行添加需要的处理 */
    }
}
void ws2812_setleds_rgbw(LED_TYPE* ledarray, uint16_t number_of_leds)
{
    // const uint16_t t0H = ((int)(0.35f / 0.0625f)) | (0x8000);
    // const uint16_t t1H = ((int)(1.36f / 0.0625f)) | (0x8000);
    // nrf_pwm_values_common_t led[RGBLED_NUM * 3 * 8 + 1];
    // nrf_pwm_values_t p_led = { .p_common = led };
    // nrf_pwm_sequence_t pwm_seq = {
    //     .values = p_led,
    //     .length = sizeof(led)
    //         / sizeof(led[0]),
    //     .repeats = 0,
    //     .end_delay = 0,
    // };
    // uint16_t* p_dat = &led[0];

    // for (int i = 0; i < number_of_leds; i++) {
    //     uint8_t mask = 1;
    //     for (int j = 0; j < 8; j++) {
    //         *p_dat++ = (ledarray[i].r & (mask << j)) ? t1H : t0H;
    //     }
    //     mask = 1;
    //     for (int j = 0; j < 8; j++) {
    //         *p_dat++ = (ledarray[i].g & (mask << j)) ? t1H : t0H;
    //     }
    //     mask = 1;
    //     for (int j = 0; j < 8; j++) {
    //         *p_dat++ = (ledarray[i].b & (mask << j)) ? t1H : t0H;
    //     }
    // }
    // *p_dat++ = 0x8000;
    // nrfx_pwm_simple_playback(&pwm0, &pwm_seq, 1, NRFX_PWM_FLAG_STOP);
}
