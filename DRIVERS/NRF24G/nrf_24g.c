#include "nrf_24g.h"
#include "nrf_drv_clock.h"
#include "ble_services.h"
#include "ble_conn_params.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "wait_api.h"
#include "action_layer.h"
#include "data_storage.h"

//注册模式需要的存储区
CONFIG_SECTION(switch_mode, 1);

/**@brief 读取前一次模式.
 *
 */
uint8_t wireless_mode_read(void)
{
    return switch_mode.data[0];
}
/**@brief 写入mode.
 *
 */
void wireless_mode_write(uint8_t val)
{
    if (switch_mode.data[0] != val)
    {
        switch_mode.data[0] = val;
        storage_write((1 << STORAGE_CONFIG));
    }
}
#define NRF_24G_TX_POWER  	NRF_ESB_TX_POWER_8DBM
#define NRF_24G_TX_BITRATE  NRF_ESB_BITRATE_1MBPS_BLE
// NRF_ESB_BITRATE_1MBPS

#define NRF_24G_DEFAULT_CHANNEL 2														
// #define NRF_24G_CHANNEL_INTERVAL 19
#define NRF_24G_DFU 0X35
//发送和接受数据结构体
static nrf_esb_config_t nrf_esb_config = NRF_ESB_DEFAULT_CONFIG;
static nrf_esb_payload_t tx_payload;
nrf_esb_payload_t tx_24g_dfu;//usb接收器进入DFU模式
static nrf_esb_payload_t rx_payload;
bool radio_tx_busy = false;
bool select_24g = false;
uint8_t rf24g_led;
static void clocks_start(void)
{
	nrf_drv_clock_hfclk_request(NULL);
}
static void nrf_esb_event_handler(nrf_esb_evt_t const *p_event)
{
	switch (p_event->evt_id)
	{
	case NRF_ESB_EVENT_TX_SUCCESS:
		break;
	case NRF_ESB_EVENT_TX_FAILED:
		(void)nrf_esb_flush_tx();
		(void)nrf_esb_start_tx();
		break;
	case NRF_ESB_EVENT_RX_RECEIVED:
		while (nrf_esb_read_rx_payload(&rx_payload) == NRF_SUCCESS)
		{
			rf24g_led = rx_payload.data[0];
		}
		break;
	}
}
static uint32_t esb_init(void)
{
	uint32_t err_code;
	uint8_t base_addr_0[4] = {0xE7, 0xE7, 0xE7, 0xE7};
	uint8_t base_addr_1[4] = {0xC2, 0xC2, 0xC2, 0xC2};
	uint8_t addr_prefix[8] = {BASE_KEY, LEFT_KEY, RIGHT_KEY, MOUSE1_KEY, MOUSE2_KEY, DFU_24G_KEY, JOY1_KEY, JOY2_KEY};
	nrf_esb_config.protocol = NRF_ESB_PROTOCOL_ESB_DPL;
	nrf_esb_config.bitrate = NRF_24G_TX_BITRATE;
	nrf_esb_config.retransmit_count = 50;
	nrf_esb_config.event_handler = nrf_esb_event_handler;
	nrf_esb_config.mode = NRF_ESB_MODE_PTX;
	nrf_esb_config.selective_auto_ack = false;
	nrf_esb_config.tx_output_power = NRF_24G_TX_POWER;
	err_code = nrf_esb_init(&nrf_esb_config);

	VERIFY_SUCCESS(err_code);

	err_code = nrf_esb_set_base_address_0(base_addr_0);
	VERIFY_SUCCESS(err_code);

	err_code = nrf_esb_set_base_address_1(base_addr_1);
	VERIFY_SUCCESS(err_code);

	err_code = nrf_esb_set_prefixes(addr_prefix, NRF_ESB_PIPE_COUNT);
	VERIFY_SUCCESS(err_code);

	err_code = nrf_esb_set_rf_channel(NRF_24G_DEFAULT_CHANNEL);
	VERIFY_SUCCESS(err_code);

	return err_code;
}
void rf24g_init(void)
{
	clocks_start();

	uint32_t err_code = esb_init();
	APP_ERROR_CHECK(err_code);
	//默认在通道0上发送
	tx_payload.pipe = BASE_PIPE;
	tx_payload.noack = false;
	tx_24g_dfu.pipe = DFU_24G_PIPE;
	tx_24g_dfu.noack = false;
	tx_24g_dfu.data[0] = NRF_24G_DFU;
	tx_24g_dfu.length = 1;
}
//通道
void update_24g_pipe(uint8_t pipe)
{
	tx_payload.pipe = pipe;
}
//信道
void update_24g_channel(uint8_t channel)
{
	uint32_t err_code = nrf_esb_set_rf_channel(channel);
	APP_ERROR_CHECK(err_code);
}

void ble_app_start(void)
{
	bool reset = false;
	// Disable low frequency clock. Readying for softdevice startup.
	nrf_drv_clock_lfclk_release();
	// Disable Gazell.
	nrf_esb_disable();
	NVIC_DisableIRQ(RADIO_IRQn);
	NVIC_ClearPendingIRQ(RADIO_IRQn);
	// Re-enable the softdevice stack.
	restart_ble();
	vTaskResume(m_softdevice_task);
	advertising_start(&reset);
}

static void ble_stack_stop(void)
{
	uint32_t err_code;

	err_code = nrf_sdh_disable_request();
	APP_ERROR_CHECK(err_code);

	ASSERT(!nrf_sdh_is_enabled());
}
void ble_app_stop(void)
{
	uint32_t err_code;
	// Stop any impending connection parameters update.
	err_code = ble_conn_params_stop();
	APP_ERROR_CHECK(err_code);

	// Disable the softdevice stack.
	vTaskSuspend(m_softdevice_task);
	ble_stack_stop();
	// Request Low frequency clock to re-enable the clock after the softdevice stops it.
	nrf_drv_clock_lfclk_request(NULL);
}

//RF发送键值
static void rf_send(enum packet_type type, uint8_t len, uint8_t *report)
{
	tx_payload.data[0] = type;
	memcpy(&tx_payload.data[1], report, len);
	tx_payload.length = len + 1;
	nrf_esb_write_payload(&tx_payload);
}

//获得LED状态
static uint8_t rf_get_keyboard_led()
{
	return rf24g_led;
}
static bool rf_queue_empty()
{
	return radio_tx_busy;
}
static bool rf_working()
{
	return select_24g;
}
static struct host_driver rf_driver = {
	.keyboard_leds = &rf_get_keyboard_led,
	.send_packet = &rf_send,
	.queue_empty = &rf_queue_empty,
	.driver_working = &rf_working,
};
// 以一个较高优先级注册USB驱动
KEYBOARD_HOST_DRIVER(6, rf_driver);
