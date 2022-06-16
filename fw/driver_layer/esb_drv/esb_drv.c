/******************************************************************************
 * HAPTIC DEVICE - HAND WEARABLE DEVICE
 *****************************************************************************/
/******************************************************************************
 * Brief description:
 * Module		:
 * Description	:
 * Tool			:
 * Chip			:
 * Version		:
 * Author		:
 * History		:
 *
 * Notes		:
 *****************************************************************************/

/******************************************************************************
 * INCLUDE
 *****************************************************************************/
#include "esb_drv.h"
#include "nrf_esb.h"
#include "nrfx.h"

/******************************************************************************
 * LOCAL DEFINITION
 *****************************************************************************/

/******************************************************************************
 * LOCAL VARIABLE DEFINITION
 *****************************************************************************/
static nrf_esb_payload_t tx_payload = NRF_ESB_CREATE_PAYLOAD(0, 0x01, 0x00);
static nrf_esb_payload_t rx_payload;
static volatile bool esb_completed = false;

/******************************************************************************
 * LOCAL FUNCTION PROTOTYPE
 *****************************************************************************/
static void EsbEventHandler(nrf_esb_evt_t const * p_event);

/******************************************************************************
 * PUBLIC VARIABLE DEFINITION
 *****************************************************************************/

/******************************************************************************
 * PUBLIC FUNCTION DEFINITION
 *****************************************************************************/
/**
 * @brief:
 * @param:
 * @return:
 */
uint32_t EsbDrvInit(void)
{
	uint8_t base_addr_0[4] = {0xE7, 0xE7, 0xE7, 0xE7};
	uint8_t base_addr_1[4] = {0xC2, 0xC2, 0xC2, 0xC2};
	uint8_t addr_prefix[8] = {0xE7, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8 };

	nrf_esb_config_t nrf_esb_config         = NRF_ESB_DEFAULT_CONFIG;
	nrf_esb_config.retransmit_count         = 4;
	nrf_esb_config.selective_auto_ack       = false;
	nrf_esb_config.protocol                 = NRF_ESB_PROTOCOL_ESB_DPL;
	nrf_esb_config.bitrate                  = NRF_ESB_BITRATE_2MBPS;
	nrf_esb_config.event_handler            = EsbEventHandler;
#ifdef CENTRAL_CONTROLLER
	nrf_esb_config.mode                     = NRF_ESB_MODE_PTX;
#endif
#ifdef HAPTIC_DEVICE
	nrf_esb_config.mode                     = NRF_ESB_MODE_PRX;
#endif

	nrf_esb_init(&nrf_esb_config);
	nrf_esb_set_base_address_0(base_addr_0);
	nrf_esb_set_base_address_1(base_addr_1);
	nrf_esb_set_prefixes(addr_prefix, 8);

	tx_payload.length  = 3;
	tx_payload.pipe    = 0;
	tx_payload.data[0] = 0x00;

	return NRFX_SUCCESS;
}

/**
 * @brief:
 * @param:
 * @return:
 */
void EsbDrvSend(uint8_t *byte, uint8_t length)
{
#ifdef CENTRAL_CONTROLLER
#endif
#ifdef HAPTIC_DEVICE
#endif
}

/**
 * @brief:
 * @param:
 * @return:
 */
void EsbDrvReceive()
{
#ifdef CENTRAL_CONTROLLER
#endif
#ifdef HAPTIC_DEVICE
#endif
}

/******************************************************************************
 * LOCAL FUNCTION DEFINITION
 *****************************************************************************/
/**
 * @brief:
 * @param:
 * @return:
 */
static void EsbEventHandler(nrf_esb_evt_t const * p_event)
{
	switch (p_event->evt_id)
	{
	case NRF_ESB_EVENT_TX_SUCCESS:
		break;
	case NRF_ESB_EVENT_TX_FAILED:
		(void) nrf_esb_flush_tx();
		break;
	case NRF_ESB_EVENT_RX_RECEIVED:
		// Get the most recent element from the RX FIFO.
		while (nrf_esb_read_rx_payload(&rx_payload) == NRFX_SUCCESS);
		break;
	}

	esb_completed = true;
}

/******************************************************************************
 * END OF FILE
 *****************************************************************************/
