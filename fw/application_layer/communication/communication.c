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
#include "communication.h"
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
void CommunicationInit(void)
{
#ifdef CENTRAL_CONTROLLER
	EsbDrvInit(ESB_DEVICE_0);
#endif
#ifdef HAPTIC_DEVICE
	EsbDrvInit(ESB_DEVICE_1);
#endif
}

/**
 * @brief:
 * @param:
 * @return:
 */
void CommunicationSend(uint8_t *byte, uint8_t length, DESTINATION_DEVICE_t destinationDevice)
{
	ESB_DEVICE_t esbDevice = ESB_DEVICE_INVALID;
	switch(destinationDevice)
	{
		case DES_DEV_THUMB:
			esbDevice = ESB_DEVICE_1;
			break;
		case DES_DEV_INDEX_FINGER:
			esbDevice = ESB_DEVICE_2;
			break;
		case DES_DEV_MIDDLE_FINGER:
			esbDevice = ESB_DEVICE_3;
			break;
		case DES_DEV_RING_FINGER:
			esbDevice = ESB_DEVICE_4;
			break;
		case DES_DEV_LITTLE_FINGER:
			esbDevice = ESB_DEVICE_5;
			break;
	}

	if(EsbDrvSend(byte, length, esbDevice) == NRF_SUCCESS)
	{
//		LogPrint("Sending packet success\n");
//		LogFlush();
	}
	else
	{
//		LogPrint("Sending packet failed\n");
//		LogFlush();
	}
}

void CommunicationReceive(void)
{

}

/******************************************************************************
 * LOCAL FUNCTION DEFINITION
 *****************************************************************************/
/**
 * @brief:
 * @param:
 * @return:
 */

/******************************************************************************
 * END OF FILE
 *****************************************************************************/
