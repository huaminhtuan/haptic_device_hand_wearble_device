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
#include "timer_drv.h"
#include "nrfx_timer.h"
#include "nrf_timer.h"

/******************************************************************************
 * LOCAL DEFINITION
 *****************************************************************************/

/******************************************************************************
 * LOCAL VARIABLE DEFINITION
 *****************************************************************************/
static nrfx_timer_t timerInst = NRFX_TIMER_INSTANCE(0);
static bool msFlag = false;

/******************************************************************************
 * LOCAL FUNCTION PROTOTYPE
 *****************************************************************************/
static void TimerEventHandler(nrf_timer_event_t event_type, void* p_context);

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
void TimerDrvInit(void)
{
	nrfx_timer_config_t config = NRFX_TIMER_DEFAULT_CONFIG;
	config.frequency = NRF_TIMER_FREQ_1MHz;
	config.mode = NRF_TIMER_MODE_TIMER;
	config.bit_width = NRF_TIMER_BIT_WIDTH_32;
	config.interrupt_priority = 7;
	config.p_context = NULL;
	nrfx_timer_init(&timerInst, &config, TimerEventHandler);
	nrfx_timer_extended_compare(&timerInst, NRF_TIMER_CC_CHANNEL0,
			nrfx_timer_ms_to_ticks(&timerInst, 1), NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

	nrfx_timer_enable(&timerInst);
}

/**
 * @brief:
 * @param:
 * @return:
 */
bool TimerIsMilisecFlagOn(void)
{
	bool tmpFlag = msFlag;
	msFlag = false;
	return tmpFlag;
}

/******************************************************************************
 * LOCAL FUNCTION DEFINITION
 *****************************************************************************/
/**
 * @brief:
 * @param:
 * @return:
 */
static void TimerEventHandler(nrf_timer_event_t event, void* context)
{
	switch (event)
	{
		case NRF_TIMER_EVENT_COMPARE0:
			msFlag = true;
			break;

		default:
			//Do nothing.
			break;
	}
}

/******************************************************************************
 * END OF FILE
 *****************************************************************************/
