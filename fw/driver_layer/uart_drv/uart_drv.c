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
    uint8_t                * txBuffer;        // Tx buffer
    volatile uint16_t        txBufferRead;    // Tx buffer read counter
    volatile uint16_t        txBufferWrite;   // Tx buffer write counter
    bool                     isTXBufferFull;  // Is TX buffer full?
    bool                     isTXBufferEmpty; // Is TX buffer empty?
    uint8_t                * rxBuffer;        // Rx buffer
    volatile uint16_t        rxBufferRead;    // Rx buffer read counter
    volatile uint16_t        rxBufferWrite;   // Rx buffer write counter
    bool                     isRXBufferFull;  // Is RX buffer full?
    bool                     isRXBufferEmpty; // Is RX buffer empty?
} UART_ControlBlock_t;

/******************************************************************************
 * LOCAL VARIABLE DEFINITION
 *****************************************************************************/
static uint8_t txBuffer[BUFFER_SIZE];
static uint8_t rxBuffer[BUFFER_SIZE];
static UART_ControlBlock_t uartControlBlock = {txBuffer, 0, 0, false, true,
		rxBuffer, 0, 0, false, true};
static nrfx_uart_t uartInst = NRFX_UART_INSTANCE(0);

/******************************************************************************
 * LOCAL FUNCTION PROTOTYPE
 *****************************************************************************/
static void UartEventHandler(nrfx_uart_event_t const * event, void *context);
static UART_ERR_CODE_t ReadFromTXBuff(uint8_t *byte);
static UART_ERR_CODE_t WriteToTXBuff(uint8_t *byte, uint8_t length);
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

UART_ERR_CODE_t UartDrvWriteToTXBuff(uint8_t *byte, uint8_t length)
{
	UART_ERR_CODE_t error_code = WriteToTXBuff(byte, length);
	return error_code;
}

/**
 * @brief : Send all bytes in transmitter buffer through UART pins
 * @param : none
 * @return: none
 */
void UartFlush(void)
{
	if(!(uartControlBlock.isTXBufferEmpty))
	{
		uint8_t byte;
		// Start transmission sequence by triggering the STARTTX task
		nrf_uart_task_trigger(uartInst.p_reg, NRF_UART_TASK_STARTTX);

		while(!(uartControlBlock.isTXBufferEmpty))
		{
			ReadFromTXBuff(&byte);
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
static UART_ERR_CODE_t ReadFromTXBuff(uint8_t *byte)
{
	// If TX buffer is empty, return error
	if(uartControlBlock.isTXBufferEmpty)
		return UART_ERR_EMPTY_BUFF;
	// Get byte from TX buffer and increase TX buffer read counter. We also
	// clear isTXBufferFull flag because the buffer will be read, that means
	// it can not be full.
	*byte = uartControlBlock.txBuffer[uartControlBlock.txBufferRead];
	uartControlBlock.txBufferRead++;
	uartControlBlock.isTXBufferFull = false;

	// If TX buffer read counter is equal to TX buffer write counter,
	// set isTXBufferEmpty flag to true to indicate that TX buffer is empty
	if(uartControlBlock.txBufferRead == BUFFER_SIZE)
		uartControlBlock.txBufferRead = 0;
	if(uartControlBlock.txBufferRead == uartControlBlock.txBufferWrite)
		uartControlBlock.isTXBufferEmpty = true;

	return UART_ERR_SUCCESS;
}

/**
 * @brief:
 * @param:
 * @return:
 */
static UART_ERR_CODE_t WriteToTXBuff(uint8_t *byte, uint8_t length)
{
	UART_ERR_CODE_t err = UART_ERR_SUCCESS;
	// If TX buffer is full, return error code
	if(uartControlBlock.isTXBufferFull)
		return UART_ERR_BYTES_ABORTED;

	// Clear isTXBufferEmpty flag because the buffer will be wrote, that means
	// it can not be empty
	uartControlBlock.isTXBufferEmpty = false;

	// Write bytes to TX buffer until TX buffer is full or all bytes are transmitted
	while(!(uartControlBlock.isTXBufferFull) && (length != 0))
	{
		uartControlBlock.txBuffer[uartControlBlock.txBufferWrite] = *byte;
		byte++;
		uartControlBlock.txBufferWrite++;
		length--;
		if(uartControlBlock.txBufferWrite == BUFFER_SIZE)
			uartControlBlock.txBufferWrite = 0;
		if(uartControlBlock.txBufferWrite == uartControlBlock.txBufferRead)
			uartControlBlock.isTXBufferFull = true;
	}

	if((uartControlBlock.isTXBufferFull) && (length != 0))
		err |= UART_ERR_BYTES_ABORTED;

	return err;
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
