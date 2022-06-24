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
#include "nrf_error.h"
#include "log.h"

/******************************************************************************
 * LOCAL DEFINITION
 *****************************************************************************/
#define BUFFER_SIZE 256
typedef struct
{
	buff_ctrl_block_t txCtrlBlock;
} ESB_ControlBlock_t;

/******************************************************************************
 * LOCAL VARIABLE DEFINITION
 *****************************************************************************/
static nrf_esb_payload_t txPayload;
static nrf_esb_payload_t rxPayload;
static ESB_DEVICE_t esbDeviceAddress = ESB_DEVICE_INVALID;

static uint8_t rxBuffer[BUFFER_SIZE];
static ESB_ControlBlock_t esbControlBlock = {.txCtrlBlock={rxBuffer, 0, 0, false, true}};

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
uint32_t EsbDrvInit(ESB_DEVICE_t esbDevice)
{
	uint8_t baseAddr0[4] = {0xE7, 0xE7, 0xE7, 0xE7};
	uint8_t baseAddr1[4] = {0xC2, 0xC2, 0xC2, 0xC2};
	uint8_t addrPrefix[8] = {0xE7, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8 };

	nrf_esb_config_t nrfEsbConfig	= NRF_ESB_DEFAULT_CONFIG;
	nrfEsbConfig.protocol			= NRF_ESB_PROTOCOL_ESB_DPL;
	nrfEsbConfig.event_handler		= EsbEventHandler;
	nrfEsbConfig.bitrate			= NRF_ESB_BITRATE_2MBPS;
	nrfEsbConfig.retransmit_delay	= 600;
	nrfEsbConfig.retransmit_count	= 4;
	nrfEsbConfig.selective_auto_ack	= false;
	nrf_esb_set_rf_channel(14);
#ifdef CENTRAL_CONTROLLER
	nrfEsbConfig.mode                     = NRF_ESB_MODE_PTX;
#endif
#ifdef HAPTIC_DEVICE
	nrfEsbConfig.mode                     = NRF_ESB_MODE_PRX;
#endif

	esbDeviceAddress = esbDevice;
	nrf_esb_init(&nrfEsbConfig);
	nrf_esb_set_base_address_0(baseAddr0);
	nrf_esb_set_base_address_1(baseAddr1);
	nrf_esb_set_prefixes(addrPrefix, NRF_ESB_PIPE_COUNT);
#ifdef HAPTIC_DEVICE
	nrf_esb_start_rx();
#endif

	return NRFX_SUCCESS;
}

/**
 * @brief:
 * @param:
 * @return:
 */
uint32_t EsbDrvSend(uint8_t *byte, uint8_t length, ESB_DEVICE_t esbDevice)
{
	int i = 0;
	txPayload.length = length;
	txPayload.pipe	 = (uint8_t)esbDevice;
	for(i=0; i<length; i++)
	{
		txPayload.data[i] = byte[i];
	}
	return nrf_esb_write_payload(&txPayload);
}

/**
 * @brief:
 * @param:
 * @return:
 */
void EsbDrvReceive(uint8_t *readByte, uint16_t *readLength)
{
	// Reset read length
	*readLength = 0;
	// Read all bytes in buffer
	while(!(esbControlBlock.txCtrlBlock.isBufferEmpty))
	{
		BuffCtrlReadFromBuff(&esbControlBlock.txCtrlBlock, BUFFER_SIZE, &readByte[*readLength]);
		(*readLength)++;
	}
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
//		LogPrint("sent!");
//		LogFlush();
		break;
	case NRF_ESB_EVENT_TX_FAILED:
		(void) nrf_esb_flush_tx();
		(void) nrf_esb_start_tx();
		break;
	case NRF_ESB_EVENT_RX_RECEIVED:
		// Get the most recent element from the RX FIFO.
		LogPrint("received!");
		if (nrf_esb_read_rx_payload(&rxPayload) == NRF_SUCCESS)
		{
#ifdef CENTRAL_CONTROLLER
#endif
#ifdef HAPTIC_DEVICE
			if(rxPayload.pipe == esbDeviceAddress)
			{
				BuffCtrlWriteToBuff(&esbControlBlock.txCtrlBlock, BUFFER_SIZE,
						rxPayload.data, rxPayload.length);
			}
#endif
		}
		break;
	}
}

/******************************************************************************
 * END OF FILE
 *****************************************************************************/
