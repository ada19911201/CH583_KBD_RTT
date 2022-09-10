#ifndef NRF_24F_H
#define NRF_24F_H

#include "nrf_esb.h"
#include "nrf_error.h"
#include "nrf_esb_error_codes.h"
#include "keyboard_host_driver.h"
#include "config.h"
#include "task_config.h"


#define BASE_KEY    0xE7
#define LEFT_KEY    0xA1
#define RIGHT_KEY   0xB2
#define MOUSE1_KEY  0xC3
#define MOUSE2_KEY  0xD4
#define DFU_24G_KEY 0xC6
#define JOY1_KEY    0xE5
#define JOY2_KEY    0xF7

#define BASE_PIPE       0
#define LEFT_PIPE       1
#define RIGHT_PIPE      2
#define MOUSE1_PIPE     3
#define MOUSE2_PIPE     4
#define DFU_24G_PIPE    5
#define JOY1_PIPE       6
#define JOY2_PIPE       7

extern nrf_esb_payload_t tx_24g_dfu;//usb接收器进入DFU模式
extern bool select_24g ;
extern bool radio_tx_busy ;
void rf24g_init(void);
void ble_app_start(void);
void ble_app_stop(void);
void rf24g_thread(void *arg);
void update_24g_pipe(uint8_t pipe);
void update_24g_channel(uint8_t channel);
uint8_t wireless_mode_read(void);
void wireless_mode_write(uint8_t val);
#endif

