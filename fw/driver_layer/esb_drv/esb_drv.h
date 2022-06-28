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

#ifndef _ESB_DRV_H
#define _ESB_DRV_H

/******************************************************************************
 * INCLUDE
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "buffer_control_drv.h"

/******************************************************************************
 * PUBLIC DEFINITION
 *****************************************************************************/
typedef enum
{
	ESB_DEVICE_0,
	ESB_DEVICE_1,
	ESB_DEVICE_2,
	ESB_DEVICE_3,
	ESB_DEVICE_4,
	ESB_DEVICE_5,
	ESB_DEVICE_6,
	ESB_DEVICE_7,
	ESB_DEVICE_NUM,
	ESB_DEVICE_INVALID,
}ESB_DEVICE_t;

/******************************************************************************
 * PUBLIC VARIABLE DECLARATION
 *****************************************************************************/

/******************************************************************************
 * PUBLIC FUNCTION PROTOTYPE
 *****************************************************************************/
uint32_t EsbDrvInit(ESB_DEVICE_t esbDevice);

uint32_t EsbDrvSend(uint8_t *byte, uint8_t length, ESB_DEVICE_t esbDevice);
void EsbDrvReceiveAllBytes(uint8_t *readByte, uint16_t *readLength);

bool EsbDrvIsDataAvai(void);

#endif
/******************************************************************************
 * END OF FILE
 *****************************************************************************/
