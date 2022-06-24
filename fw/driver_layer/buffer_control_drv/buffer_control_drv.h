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

#ifndef _BUFFER_CONTROL_DRV_H
#define _BUFFER_CONTROL_DRV_H

/******************************************************************************
 * INCLUDE
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/******************************************************************************
 * PUBLIC DEFINITION
 *****************************************************************************/
typedef struct
{
	uint8_t                * buffer;             // Buffer pointer
	volatile uint16_t        bufferReadCounter;  // Buffer read counter
	volatile uint16_t        bufferWriteCounter; // Buffer write counter
	bool                     isBufferFull;       // Is buffer full?
	bool                     isBufferEmpty;      // Is buffer empty?
}buff_ctrl_block_t;

typedef enum
{
	BUFF_CTRL_ERR_SUCCESS,
	BUFF_CTRL_ERR_EMPTY_BUFF,
	BUFF_CTRL_ERR_BYTES_ABORTED,
} BUFF_CTRL_ERR_CODE_t;

/******************************************************************************
 * PUBLIC VARIABLE DECLARATION
 *****************************************************************************/

/******************************************************************************
 * PUBLIC FUNCTION PROTOTYPE
 *****************************************************************************/
BUFF_CTRL_ERR_CODE_t BuffCtrlReadFromBuff(buff_ctrl_block_t *buffCtrlBlock,
		uint16_t buff_size, uint8_t *readByte);
BUFF_CTRL_ERR_CODE_t BuffCtrlWriteToBuff(buff_ctrl_block_t *buffCtrlBlock,
		uint16_t buffSize, uint8_t *writeByte, uint8_t length);

#endif
/******************************************************************************
 * END OF FILE
 *****************************************************************************/
