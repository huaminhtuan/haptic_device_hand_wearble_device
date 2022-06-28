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
	LogFlush();
	uint8_t byte[255];
	uint16_t len;
	if(CommunicationReceive(byte, &len))
	{
		LogPrint((const char *)byte);
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

/******************************************************************************
 * END OF FILE
 *****************************************************************************/
