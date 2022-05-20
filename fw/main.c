#include <nrf52840.h>
#include "nrf.h"
#include "nrf_esb.h"
#include "nrf_error.h"
#include "sdk_macros.h"
#include "app_error.h"

#define CENTRAL_CONTROLLER
//#define HAPTIC_DEVICE

static nrf_esb_payload_t tx_payload = NRF_ESB_CREATE_PAYLOAD(0, 0x01, 0x00);
static nrf_esb_payload_t rx_payload;
static volatile bool esb_completed = false;

void nrf_esb_event_handler(nrf_esb_evt_t const * p_event)
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
            while (nrf_esb_read_rx_payload(&rx_payload) == NRF_SUCCESS) ;
            break;
    }

    esb_completed = true;
}

uint32_t esb_init( void )
{
    uint32_t err_code;
    uint8_t base_addr_0[4] = {0xE7, 0xE7, 0xE7, 0xE7};
    uint8_t base_addr_1[4] = {0xC2, 0xC2, 0xC2, 0xC2};
    uint8_t addr_prefix[8] = {0xE7, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8 };

    nrf_esb_config_t nrf_esb_config         = NRF_ESB_DEFAULT_CONFIG;
    nrf_esb_config.retransmit_count         = 4;
    nrf_esb_config.selective_auto_ack       = false;
    nrf_esb_config.protocol                 = NRF_ESB_PROTOCOL_ESB_DPL;
    nrf_esb_config.bitrate                  = NRF_ESB_BITRATE_2MBPS;
    nrf_esb_config.event_handler            = nrf_esb_event_handler;
#ifdef CENTRAL_CONTROLLER
    nrf_esb_config.mode                     = NRF_ESB_MODE_PTX;
#endif
#ifdef HAPTIC_DEVICE
    nrf_esb_config.mode                     = NRF_ESB_MODE_PRX;
#endif

    err_code = nrf_esb_init(&nrf_esb_config);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_base_address_0(base_addr_0);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_base_address_1(base_addr_1);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_prefixes(addr_prefix, 8);
    VERIFY_SUCCESS(err_code);

    tx_payload.length  = 3;
    tx_payload.pipe    = 0;
    tx_payload.data[0] = 0x00;

    return NRF_SUCCESS;
}

void clocks_start( void )
{
    // Start HFCLK and wait for it to start.
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART = 1;
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
}

int main(void)
{
	uint32_t err_code;
	// Initialize
	clocks_start();
	err_code = esb_init();
	APP_ERROR_CHECK(err_code);
	return 0;
}
