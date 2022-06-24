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
#include "uart_drv.h"
#include "nrfx_uart.h"

/******************************************************************************
 * LOCAL DEFINITION
 *****************************************************************************/
#define UART_TX_PIN 25
#define UART_RX_PIN 24

#define BUFFER_SIZE	500 //bytes

typedef struct
{
	buff_ctrl_block_t txCtrlBlock;
	buff_ctrl_block_t rxCtrlBlock;
} UART_ControlBlock_t;

/******************************************************************************
 * LOCAL VARIABLE DEFINITION
 *****************************************************************************/
static uint8_t txBuffer[BUFFER_SIZE];
static uint8_t rxBuffer[BUFFER_SIZE];
static UART_ControlBlock_t uartControlBlock = {.txCtrlBlock={txBuffer, 0, 0, false, true},
											   .rxCtrlBlock={rxBuffer, 0, 0, false, true}};
static nrfx_uart_t uartInst = NRFX_UART_INSTANCE(0);

/******************************************************************************
 * LOCAL FUNCTION PROTOTYPE
 *****************************************************************************/
static void UartEventHandler(nrfx_uart_event_t const * event, void *context);
static uint32_t UartDrvPut(uint8_t byte);

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
uint32_t UartDrvInit(void)
{
	nrfx_uart_config_t config = NRFX_UART_DEFAULT_CONFIG;
    config.pseltxd = UART_TX_PIN;
    config.pselrxd = UART_RX_PIN;
    config.hwfc = NRF_UART_HWFC_DISABLED;
    config.parity = NRF_UART_PARITY_EXCLUDED;
	config.baudrate = NRF_UART_BAUDRATE_460800;
	config.interrupt_priority = 7;
	uint32_t error_code = nrfx_uart_init(&uartInst, &config, UartEventHandler);
	nrfx_uart_rx(&uartInst, rxBuffer, BUFFER_SIZE);
	return error_code;
}

BUFF_CTRL_ERR_CODE_t UartDrvWriteToTXBuff(uint8_t *byte, uint8_t length)
{
	return BuffCtrlWriteToBuff(&uartControlBlock.txCtrlBlock, BUFFER_SIZE, byte, length);
}

/**
 * @brief : Send all bytes in transmitter buffer through UART pins
 * @param : none
 * @return: none
 */
void UartFlush(void)
{
	if(!(uartControlBlock.txCtrlBlock.isBufferEmpty))
	{
		uint8_t byte;
		// Start transmission sequence by triggering the STARTTX task
		nrf_uart_task_trigger(uartInst.p_reg, NRF_UART_TASK_STARTTX);

		while(!(uartControlBlock.txCtrlBlock.isBufferEmpty))
		{
			BuffCtrlReadFromBuff(&uartControlBlock.txCtrlBlock, BUFFER_SIZE, &byte);
			UartDrvPut(byte);
		}
		nrf_uart_task_trigger(uartInst.p_reg, NRF_UART_TASK_STOPTX);
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
static void UartEventHandler(nrfx_uart_event_t const * event, void *context)
{
	if (event->type == NRFX_UART_EVT_RX_DONE)
	{
	}
	else if (event->type == NRFX_UART_EVT_ERROR)
	{
	}
	else if (event->type == NRFX_UART_EVT_TX_DONE)
	{
	}
}

/**
 * @brief:
 * @param:
 * @return:
 */
static uint32_t UartDrvPut(uint8_t byte)
{
	NRF_UART0->TXD = byte;
	uint32_t timeout = 100;
	while(!nrf_uart_event_check(uartInst.p_reg, NRF_UART_EVENT_TXDRDY) && (timeout != 0))
	{
		timeout--;
	}

	if (timeout == 0)
		return NRFX_ERROR_TIMEOUT;
	else
	{
		nrf_uart_event_clear(uartInst.p_reg, NRF_UART_EVENT_TXDRDY);
		return NRFX_SUCCESS;
	}
}

/******************************************************************************
 * END OF FILE
 *****************************************************************************/
