/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2020 Ploopy Corporation
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

#include "pmw3360.h"
#include "wait.h"
#include "debug.h"
#include "print.h"
#include "wait_api.h"
#include "task_config.h"
#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrfx_spim.h"
#include "xprintf.h"
#include "report.h"
#include "host.h"

#include PMW3360_FIRMWARE_H
//SPIM驱动程序实例ID,ID和外设编号对应，0:SPIM0 1:SPIM1 2:SPIM2 3:SPIM3
#define SPI_INSTANCE 3
//定义SPI驱动呢程序实例
static const nrfx_spim_t spi = NRFX_SPIM_INSTANCE(SPI_INSTANCE);
//SPI传输完成标志
static volatile bool spi_xfer_done;
//SPI发送缓存数组，使用EasyDMA时一定要定义为static类型
static uint8_t spi_tx_buf[PMW3360_LENGTH];
//SPI接收缓存数组，使用EasyDMA时一定要定义为static类型
static uint8_t spi_rx_buf[PMW3360_LENGTH];

//定义SPIM传输描述符结构体
static nrfx_spim_xfer_desc_t spim3_xfer;
static report_mouse_t g_mouse_report = {};
// Registers
// clang-format off
#define REG_Product_ID                 0x00
#define REG_Revision_ID                0x01
#define REG_Motion                     0x02
#define REG_Delta_X_L                  0x03
#define REG_Delta_X_H                  0x04
#define REG_Delta_Y_L                  0x05
#define REG_Delta_Y_H                  0x06
#define REG_SQUAL                      0x07
#define REG_Raw_Data_Sum               0x08
#define REG_Maximum_Raw_data           0x09
#define REG_Minimum_Raw_data           0x0A
#define REG_Shutter_Lower              0x0B
#define REG_Shutter_Upper              0x0C
#define REG_Control                    0x0D
#define REG_Config1                    0x0F
#define REG_Config2                    0x10
#define REG_Angle_Tune                 0x11
#define REG_Frame_Capture              0x12
#define REG_SROM_Enable                0x13
#define REG_Run_Downshift              0x14
#define REG_Rest1_Rate_Lower           0x15
#define REG_Rest1_Rate_Upper           0x16
#define REG_Rest1_Downshift            0x17
#define REG_Rest2_Rate_Lower           0x18
#define REG_Rest2_Rate_Upper           0x19
#define REG_Rest2_Downshift            0x1A
#define REG_Rest3_Rate_Lower           0x1B
#define REG_Rest3_Rate_Upper           0x1C
#define REG_Observation                0x24
#define REG_Data_Out_Lower             0x25
#define REG_Data_Out_Upper             0x26
#define REG_Raw_Data_Dump              0x29
#define REG_SROM_ID                    0x2A
#define REG_Min_SQ_Run                 0x2B
#define REG_Raw_Data_Threshold         0x2C
#define REG_Config5                    0x2F
#define REG_Power_Up_Reset             0x3A
#define REG_Shutdown                   0x3B
#define REG_Inverse_Product_ID         0x3F
#define REG_LiftCutoff_Tune3           0x41
#define REG_Angle_Snap                 0x42
#define REG_LiftCutoff_Tune1           0x4A
#define REG_Motion_Burst               0x50
#define REG_LiftCutoff_Tune_Timeout    0x58
#define REG_LiftCutoff_Tune_Min_Length 0x5A
#define REG_SROM_Load_Burst            0x62
#define REG_Lift_Config                0x63
#define REG_Raw_Data_Burst             0x64
#define REG_LiftCutoff_Tune2           0x65


// clang-format on

#ifndef MAX_CPI
#    define MAX_CPI 0x77  // limits to 0--119, should be max cpi/100
#endif

bool _inBurst = false;

#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define constrain_hid(amt) ((amt) < -127 ? -127 : ((amt) > 127 ? 127 : (amt)))

void spi_write_adv(uint8_t reg_addr, uint8_t data) {
    if (reg_addr != REG_Motion_Burst) {
        _inBurst = false;
    }

    spi_write(reg_addr | 0x80);
    spi_write(data);

    // tSCLK-NCS for write operation
    delay_us(180);
}

uint8_t spi_read_adv(uint8_t reg_addr) {
    spi_write(reg_addr);
    delay_us(160);
    spi_read();
    uint8_t data = spi_rx_buf[0];
    delay_us(20);
    return data;
}

void pmw3360_set_cpi(uint16_t cpi) {
    uint8_t cpival = constrain((cpi / 100) - 1, 0, MAX_CPI);
    spi_write_adv(REG_Config1, cpival);
}

uint16_t pmw3360_get_cpi(void) {
    uint8_t cpival = spi_read_adv(REG_Config1);
    return (uint16_t)((cpival + 1) & 0xFF) * 100;
}

bool pmw3360_init(void) {

    spi_init();
    _inBurst = false;
    PMW3360_CS_HIGH;
    delay_us(300);
    PMW3360_CS_LOW;
    spi_write_adv(REG_Shutdown, 0xb6);  // Shutdown first
    PMW3360_CS_HIGH;
    delay_us(300);
//重启传感器
    PMW3360_CS_LOW;
	delay_us(40);
	PMW3360_CS_HIGH;
	delay_us(40);
    PMW3360_CS_LOW;
    spi_write_adv(REG_Power_Up_Reset, 0x5a);
    PMW3360_CS_HIGH;
    delay_us(500);

    PMW3360_CS_LOW;
	spi_write_adv(0x3d, 0x95);
	PMW3360_CS_HIGH;
	delay_us(100);
	PMW3360_CS_LOW;
	spi_write_adv(0x3d, 0x15);
	PMW3360_CS_HIGH;
	delay_us(100);

    PMW3360_CS_LOW;
    spi_read_adv(REG_Motion);
    spi_read_adv(REG_Delta_X_L);
    spi_read_adv(REG_Delta_X_H);
    spi_read_adv(REG_Delta_Y_L);
    spi_read_adv(REG_Delta_Y_H);

	spi_write_adv(0x10, 0x00); // disable rest mode
	spi_write_adv(0x22, 0x00); // ???

    pmw3360_upload_firmware();

    // spi_stop_adv();
    delay_us(100);
    pmw3360_set_cpi(PMW3360_CPI);

    delay_us(100);

    spi_write_adv(REG_Config2, 0x00);

    spi_write_adv(REG_Angle_Tune, constrain(ROTATIONAL_TRANSFORM_ANGLE, -30, 30));

    spi_write_adv(REG_Lift_Config, PMW3360_LIFTOFF_DISTANCE);

    bool init_success = pmw3360_check_signature();

    // writePinLow(PMW3360_CS_PIN);

    return init_success;
}

void pmw3360_upload_firmware(void) 
{
    spi_write_adv(REG_SROM_Enable, 0x1d);
	PMW3360_CS_HIGH;
	delay_us(100);
	PMW3360_CS_LOW;
    spi_write_adv(REG_SROM_Enable, 0x18);

    spi_write(REG_SROM_Load_Burst | 0x80);
    unsigned char c;
    for (int i = 0; i < FIRMWARE_LENGTH; i++) {
        c = (unsigned char)pgm_read_byte(firmware_data + i);
        delay_us(15);
        spi_write(c);
    }
    delay_us(18);
    PMW3360_CS_HIGH;
    delay_us(200);
    PMW3360_CS_LOW;
	spi_write_adv(0x10, 0x20); // 0x20 enables rest mode after ~10s of inactivity
	spi_write_adv(0x14, 0xff); // how long to wait before going to rest mode. 0xff is max (~10 seconds)
	spi_write_adv(0x15, 0x00); // default
	spi_write_adv(0x16, 0x00); // default
	spi_write_adv(0x17, 0xff);
	spi_write_adv(0x18, 0x63); // default
	spi_write_adv(0x19, 0x00); // default
	spi_write_adv(0x1a, 0x5e);
	spi_write_adv(0x1b, 0x8f);
	spi_write_adv(0x1c, 0x01);
	PMW3360_CS_HIGH;
    delay_us(100); // arbitrary padding
	PMW3360_CS_LOW;
	spi_write_adv(0x3d, 0x93);
	spi_write_adv(0x3d, 0x13); // increase this to increase the clock frequency during run mode
	PMW3360_CS_HIGH;
	delay_us(100); // arbitrary padding
	PMW3360_CS_LOW;
	spi_write_adv(0x4f, 0x91);
	spi_write_adv(0x4f, 0x11); // increase this to increase the clock frequency during rest mode
	PMW3360_CS_HIGH;
	delay_us(100); // arbitrary padding

	PMW3360_CS_LOW;
	spi_write_adv(0x0f, MAX_CPI);
	spi_write_adv(0x42, 0x00); // angle snapping
	//spi_write_adv(0x63, 0x03); // 3mm lod
	PMW3360_CS_HIGH;
    // spi_read_adv(REG_SROM_ID);

    // spi_write_adv(REG_Config2, 0x00);
    // delay_us(10);
}

bool pmw3360_check_signature(void) {
    uint8_t pid      = spi_read_adv(REG_Product_ID);
    uint8_t iv_pid   = spi_read_adv(REG_Inverse_Product_ID);
    uint8_t SROM_ver = spi_read_adv(REG_SROM_ID);
    return (pid == firmware_signature[0] && iv_pid == firmware_signature[1] && SROM_ver == firmware_signature[2]);  // signature for SROM 0x04
}

report_pmw3360_t pmw3360_read_burst(void) {
    PMW3360_CS_LOW;
    if (!_inBurst) {
        spi_write_adv(REG_Motion_Burst, 0x00);
        _inBurst = true;
    }
    // spi_start_adv();
    spi_write(REG_Motion_Burst);
    delay_us(35);  // waits for tSRAD

    spi_read();
    delay_us(20);
    PMW3360_CS_HIGH;
    report_pmw3360_t data = {0};
    data.motion = spi_rx_buf[0];
    data.dx  = spi_rx_buf[2];
    data.mdx = spi_rx_buf[3];
    data.dy  = spi_rx_buf[4];
    data.mdy = spi_rx_buf[5];
    data.isMotion    = (data.motion & 0x80) != 0;
    data.isOnSurface = (data.motion & 0x08) == 0;
    data.dx |= (data.mdx << 8);
    // data.dx = data.dx * -1;
    data.dy |= (data.mdy << 8);
    // data.dy = data.dy * -1;
    // xprintf("data.dx = %d,data.dy = %d",data.dx,data.dy);
    // spi_stop();
    if (data.motion & 0b111) {  // panic recovery, sometimes burst mode works weird.
        _inBurst = false;
    }

    return data;
}

static void send_mouse_report(report_pmw3360_t data){
    if (data.dx!=0||data.dy!=0) 
    {   
        g_mouse_report.x = constrain_hid(data.dx);
        g_mouse_report.y = constrain_hid(data.dy);
        host_mouse_send(&g_mouse_report);
    }
}
void pmw3360_task_thread (void *p_context)
{
    pmw3360_init();
    while (1)
    {
        send_mouse_report(pmw3360_read_burst());
        wait_ms(PMW3360_TASK_TIMERS);
    }
}

/*****************************************************************************
** 描  述：写入一个字节
** 参  数：Dat：待写入的数据
** 返回值：无
******************************************************************************/
void  spi_write(uint8_t Dat)
{
    spi_tx_buf[0] = Dat;
    //设置SPI传输描述符
    spim3_xfer.p_tx_buffer = spi_tx_buf;
    spim3_xfer.tx_length = 1;
    spim3_xfer.p_rx_buffer = spi_rx_buf;
    spim3_xfer.rx_length = 0;
    //传输完成标志设置为false
    spi_xfer_done = false;
    	//拉低CS，使能W25Q128FV
    //启动数据传输
    ret_code_t  err_code =nrfx_spim_xfer(&spi, &spim3_xfer, 0);
    APP_ERROR_CHECK(err_code);
    //等待SPI传输完成
    while (!spi_xfer_done)
    {
    }
}
void  spi_read()
{
    //设置SPI传输描述符
    spim3_xfer.p_tx_buffer = spi_tx_buf;
    spim3_xfer.tx_length = 0;
    spim3_xfer.p_rx_buffer = spi_rx_buf;
    spim3_xfer.rx_length = 12;
    //传输完成标志设置为false
    spi_xfer_done = false;
    //启动数据传输
    ret_code_t  err_code =nrfx_spim_xfer(&spi, &spim3_xfer, 0);
    APP_ERROR_CHECK(err_code);
    //等待SPI传输完成
    while (!spi_xfer_done)
    {
    }
}

//SPIM事件处理函数
void spi_event_handler(nrfx_spim_evt_t const *p_event,
                       void *p_context)
{
    //设置SPIM传输完成
    spi_xfer_done = true;
}

//初始化spi
void spi_init(void)
{
    nrf_gpio_cfg_output(PMW3360_CS_PIN);
    nrf_gpio_cfg_output(PMW3360_RESET_PIN);
    PMW3360_RESET_HIGH;
    PMW3360_CS_HIGH;
    //使用默认配置参数初始化SPI配置结构体
    nrfx_spim_config_t spi_config = NRFX_SPIM_DEFAULT_CONFIG;
    //重写SPIM信号连接的引脚配置
    spi_config.ss_pin = NRFX_SPIM_PIN_NOT_USED;
    spi_config.miso_pin = PMW3360_MISO_PIN;
    spi_config.mosi_pin = PMW3360_MOSI_PIN;
    spi_config.sck_pin = PMW3360_SCK_PIN;
    spi_config.frequency = NRF_SPIM_FREQ_4M;
    spi_config.mode = NRF_SPIM_MODE_3;

    //初始化SPIM
    APP_ERROR_CHECK(nrfx_spim_init(&spi, &spi_config, spi_event_handler, NULL));
}
