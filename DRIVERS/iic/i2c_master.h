#pragma once

#include <stdint.h>
#include <stdbool.h>

#if defined(RGB_TOP_ENABLE) || defined(SSD1306_ENABLE)

extern uint16_t num_yyy;
typedef int8_t i2c_status_t;

void         i2c_init(void);
void         i2c_start(void);
void         i2c_stop(void);
i2c_status_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length, uint16_t timeout);
i2c_status_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length);

#endif