#include <nrf52840.h>
#include "nrfx_config.h"
#include "nrf_uart.h"
//#include "nrf.h"
//#include "nrf_esb.h"
#include "nrf_error.h"
#include "sdk_macros.h"
//#include "app_error.h"
#include "nrfx_uart.h"

#define CENTRAL_CONTROLLER
//#define HAPTIC_DEVICE

#define TX_PIN 25
#define RX_PIN 24

//static nrf_esb_payload_t tx_payload = NRF_ESB_CREATE_PAYLOAD(0, 0x01, 0x00);
//static nrf_esb_payload_t rx_payload;
static volatile bool esb_completed = false;

//static uint8_t tx_buffer[1];
static uint8_t rx_buffer[1];
static nrfx_uart_t uart_inst = NRFX_UART_INSTANCE(0);
//static bool uarttx_in_progress = false;

//void nrf_esb_event_handler(nrf_esb_evt_t const * p_event)
//{
//    switch (p_event->evt_id)
//    {
//        case NRF_ESB_EVENT_TX_SUCCESS:
//            break;
//        case NRF_ESB_EVENT_TX_FAILED:
//            (void) nrf_esb_flush_tx();
//            break;
//        case NRF_ESB_EVENT_RX_RECEIVED:
//            // Get the most recent element from the RX FIFO.
//            while (nrf_esb_read_rx_payload(&rx_payload) == NRF_SUCCESS) ;
//            break;
//    }

//    esb_completed = true;
//}

uint32_t esb_init( void )
{
//    uint32_t err_code;
//    uint8_t base_addr_0[4] = {0xE7, 0xE7, 0xE7, 0xE7};
//    uint8_t base_addr_1[4] = {0xC2, 0xC2, 0xC2, 0xC2};
//    uint8_t addr_prefix[8] = {0xE7, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8 };
//
//    nrf_esb_config_t nrf_esb_config         = NRF_ESB_DEFAULT_CONFIG;
//    nrf_esb_config.retransmit_count         = 4;
//    nrf_esb_config.selective_auto_ack       = false;
//    nrf_esb_config.protocol                 = NRF_ESB_PROTOCOL_ESB_DPL;
//    nrf_esb_config.bitrate                  = NRF_ESB_BITRATE_2MBPS;
//    nrf_esb_config.event_handler            = nrf_esb_event_handler;
//#ifdef CENTRAL_CONTROLLER
//    nrf_esb_config.mode                     = NRF_ESB_MODE_PTX;
//#endif
//#ifdef HAPTIC_DEVICE
//    nrf_esb_config.mode                     = NRF_ESB_MODE_PRX;
//#endif
//
//    err_code = nrf_esb_init(&nrf_esb_config);
//    VERIFY_SUCCESS(err_code);
//
//    err_code = nrf_esb_set_base_address_0(base_addr_0);
//    VERIFY_SUCCESS(err_code);
//
//    err_code = nrf_esb_set_base_address_1(base_addr_1);
//    VERIFY_SUCCESS(err_code);
//
//    err_code = nrf_esb_set_prefixes(addr_prefix, 8);
//    VERIFY_SUCCESS(err_code);
//
//    tx_payload.length  = 3;
//    tx_payload.pipe    = 0;
//    tx_payload.data[0] = 0x00;

    return NRFX_SUCCESS;
}

void clocks_start( void )
{
    // Start HFCLK and wait for it to start.
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART = 1;
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
}

void uart_event_handler(nrfx_uart_event_t const * p_event, void *p_context)
{
	if (p_event->type == NRFX_UART_EVT_RX_DONE)
	{
//        // Received bytes counter has to be checked, because there could be event from RXTO interrupt
//        if (p_event->data.rxtx.bytes)
//        {
//            app_uart_evt_t app_uart_event;
//            app_uart_event.evt_type   = APP_UART_DATA;
//            app_uart_event.data.value = p_event->data.rxtx.p_data[0];
//            rx_done = true;
//            m_event_handler(&app_uart_event);
//        }
//        (void)nrf_drv_uart_rx(&app_uart_inst, rx_buffer, 1);
	}
	else if (p_event->type == NRFX_UART_EVT_ERROR)
	{
//        app_uart_evt_t app_uart_event;
//        app_uart_event.evt_type                 = APP_UART_COMMUNICATION_ERROR;
//        app_uart_event.data.error_communication = p_event->data.error.error_mask;
//        (void)nrf_drv_uart_rx(&app_uart_inst, rx_buffer, 1);
//        m_event_handler(&app_uart_event);
	}
	else if (p_event->type == NRFX_UART_EVT_TX_DONE)
	{
//       // Last byte from FIFO transmitted, notify the application.
//       // Notify that new data is available if this was first byte put in the buffer.
//       app_uart_evt_t app_uart_event;
//       app_uart_event.evt_type = APP_UART_TX_EMPTY;
//       m_event_handler(&app_uart_event);
	}
}

uint32_t uart_init()
{
	nrfx_uart_config_t config = NRFX_UART_DEFAULT_CONFIG;
    config.pseltxd = TX_PIN;
    config.pselrxd = RX_PIN;
    config.hwfc = NRF_UART_HWFC_DISABLED;
    config.parity = NRF_UART_PARITY_EXCLUDED;
	config.baudrate = NRF_UART_BAUDRATE_9600;
	config.interrupt_priority = 7;
	uint32_t error_code = nrfx_uart_init(&uart_inst, &config, uart_event_handler);
	NRF_UART0->CONFIG = NRF_UART_HWFC_DISABLED | NRF_UART_PARITY_EXCLUDED;
	NRF_UART0->BAUDRATE = NRF_UART_BAUDRATE_9600;
	NRF_UART0->ENABLE = 0x4;
	nrf_uart_event_clear(uart_inst.p_reg, NRF_UART_EVENT_TXDRDY);
	nrfx_uart_rx(&uart_inst, rx_buffer,1);
	return error_code;
}

uint32_t uartPut(uint8_t byte)
{
	NRF_UART0->TXD = byte;
	uint32_t timeout = 1000;
	while(!nrf_uart_event_check(uart_inst.p_reg, NRF_UART_EVENT_TXDRDY) && (timeout != 0))
	{
		timeout--;
	}

	if (timeout == 0)
		return NRFX_SUCCESS;
	else
	{
		nrf_uart_event_clear(uart_inst.p_reg, NRF_UART_EVENT_TXDRDY);
		return NRFX_ERROR_TIMEOUT;
	}
}

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE * f)
#endif

PUTCHAR_PROTOTYPE
{
	uartPut(ch);
    return ch;
}

int main(void)
{
	uint32_t err_code;
	// Initialize
//	clocks_start();
//	err_code = esb_init();
//	APP_ERROR_CHECK(err_code);

	uart_init();
//	while(nrfx_uart_tx_in_progress(&uart_inst));
	err_code = uartPut(0x65);
	VERIFY_SUCCESS(err_code);
//	while(nrfx_uart_tx_in_progress(&uart_inst));
	uartPut(0x00);
//	while(nrfx_uart_tx_in_progress(&uart_inst));
	uartPut(0x0A);
//	printf("start");
	while(1)
	{

	}
	return 0;
}
