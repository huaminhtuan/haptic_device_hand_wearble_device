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
#include "nrf_error.h"
#include "system_management.h"
#include "log.h"
#include "esb_drv.h"

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
void SystemManagementInit(void)
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
	LogFlush();

	ClockStart();
#ifdef CENTRAL_CONTROLLER
	EsbDrvInit(ESB_DEVICE_0);
	uint8_t str[] = "test\n";
	if(EsbDrvSend(str, 6, ESB_DEVICE_1) == NRF_SUCCESS)
	{
		LogPrint("ESB Sent okay\n");
		LogFlush();
	}
	else
	{
		LogPrint("Sending packet failed\n");
		LogFlush();
	}
#endif
#ifdef HAPTIC_DEVICE
	EsbDrvInit(ESB_DEVICE_1);
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
static void ClockStart(void)
{
    // Start HFCLK and wait for it to start.
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART = 1;
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
}

/******************************************************************************
 * END OF FILE
 *****************************************************************************/
