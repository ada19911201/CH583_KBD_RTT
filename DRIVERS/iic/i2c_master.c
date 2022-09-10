#include <string.h>

#include "config.h"
#include "i2c_master.h"
#include "CH58x_common.h"

#ifndef RGB_I2C_SCL
#    define RGB_I2C_SCL 32
#endif
#ifndef RGB_I2C_SDA
#    define RGB_I2C_SDA 22
#endif
#ifndef CONFIG_I2C_FREQ
    #define CONFIG_I2C_FREQ NRF_TWIM_FREQ_400K
#endif


#define RxAdderss     DRIVER_ADDR_1
#define TWI_ADDR        0x53

uint16_t num_yyy = 0;
//初始化I2CS
void i2c_init(void) {
    GPIOB_ModeCfg( GPIO_Pin_21 | GPIO_Pin_20, GPIO_ModeIN_PU);
    I2C_Init(I2C_Mode_I2C, 400000, I2C_DutyCycle_16_9, I2C_Ack_Disable, I2C_AckAddr_7bit, TWI_ADDR);
}

//i2c启动
void i2c_start(void){
    while(I2C_GetFlagStatus(I2C_FLAG_BUSY) != RESET);
    I2C_GenerateSTART(ENABLE);
}
//i2c停止
void i2c_stop(void) {
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(ENABLE);
}
//i2c开始传输
i2c_status_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length, uint16_t timeout) {
    i2c_start();
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(address, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    for (uint8_t i = 0; i < length; i++)
    {
        if(I2C_GetFlagStatus(I2C_FLAG_TXE) != RESET)
        I2C_SendData(data[i]);
    }
    i2c_stop();
    return 0;
}
//i2c开始接收
i2c_status_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length) {
    // twim_done = false;
    // ret_code_t err_code = nrfx_twim_rx(&m_twim_master,address >> 1, data, length);
    // APP_ERROR_CHECK(err_code); 
    // while (twim_done == true);
    // return 0;
}
