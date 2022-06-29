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
#include "system.h"
#include "nrf_error.h"
#include "log.h"
#include "communication.h"
#include "timer_drv.h"

/******************************************************************************
 * LOCAL DEFINITION
 *****************************************************************************/

/******************************************************************************
 * LOCAL VARIABLE DEFINITION
 *****************************************************************************/

/******************************************************************************
 * LOCAL FUNCTION PROTOTYPE
 *****************************************************************************/
static void ClockStart(void);

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
void SystemAppInit(void)
{
	LogInit();
	LogPrint("\n\n\n");
	LogPrint("HAPTIC DEVICE - HAND WEARABLE DEVICE PROJECT\n");
#ifdef CENTRAL_CONTROLLER
	LogPrint("Role: central controller\n");
#endif
#ifdef HAPTIC_DEVICE
	LogPrint("Role: haptic device\n");
#endif
	LogPrint("Initializing...\n");

	ClockStart();
	CommunicationInit();

	TimerDrvInit();

	LogPrint("Initialization done!\n");
#ifdef CENTRAL_CONTROLLER
	uint8_t str[] = "test\n";
	CommunicationSend(str, 6, DES_DEV_THUMB);
#endif
	LogFlush();
}

/**
 * @brief:
 * @param:
 * @return:
 */
void SystemControl(void)
{
	static uint16_t msTickCounter = 0;

	if(TimerIsMilisecFlagOn())
	{
		msTickCounter++;
	}

	// 10ms tasks
	if((msTickCounter%10==0))
	{
	}
	// 100ms tasks
	if((msTickCounter%100==0)==true)
	{
		msTickCounter = 0;
		uint8_t byte[255];
		uint16_t len;
		if(CommunicationReceive(byte, &len))
		{
			LogPrint((const char *)byte);
		}
		LogFlush();
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
static void ClockStart(void)
{
    // Start HFCLK and wait for it to start.
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART = 1;
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
}

void HardFault_Handler(void)
{
	LogPrint("Hard Fault\n");
	LogFlush();
	while(1);
}

/******************************************************************************
 * END OF FILE
 *****************************************************************************/
