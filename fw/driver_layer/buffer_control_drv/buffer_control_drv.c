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
#include "buffer_control_drv.h"

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
BUFF_CTRL_ERR_CODE_t BuffCtrlReadFromBuff(buff_ctrl_block_t *buffCtrlBlock,
		uint16_t buffSize, uint8_t *readByte)
{
	// If buffer is empty, return error
	if(buffCtrlBlock->isBufferEmpty)
		return BUFF_CTRL_ERR_EMPTY_BUFF;
	// Get byte from buffer and increase buffer read counter. We also
	// clear isBufferFull flag because the buffer will be read, that means
	// it can not be full.
	*readByte = buffCtrlBlock->buffer[buffCtrlBlock->bufferReadCounter];
	buffCtrlBlock->bufferReadCounter++;
	buffCtrlBlock->isBufferFull = false;

	// If buffer read counter is equal to buffer write counter,
	// set isBufferEmpty flag to true to indicate that buffer is empty
	if(buffCtrlBlock->bufferReadCounter == buffSize)
		buffCtrlBlock->bufferReadCounter = 0;
	if(buffCtrlBlock->bufferReadCounter == buffCtrlBlock->bufferWriteCounter)
		buffCtrlBlock->isBufferEmpty = true;

	return BUFF_CTRL_ERR_SUCCESS;
}

/**
 * @brief:
 * @param:
 * @return:
 */
BUFF_CTRL_ERR_CODE_t BuffCtrlWriteToBuff(buff_ctrl_block_t *buffCtrlBlock,
		uint16_t buffSize, uint8_t *writeByte, uint8_t length)
{
	// If buffer is full, return error code
	if(buffCtrlBlock->isBufferFull)
		return BUFF_CTRL_ERR_BYTES_ABORTED;

	// Clear isBufferEmpty flag because the buffer will be wrote, that means
	// it can not be empty
	buffCtrlBlock->isBufferEmpty = false;

	// Write bytes to buffer until buffer is full or all bytes are transmitted
	while(!(buffCtrlBlock->isBufferFull) && (length != 0))
	{
		buffCtrlBlock->buffer[buffCtrlBlock->bufferWriteCounter] = *writeByte;
		writeByte++;
		buffCtrlBlock->bufferWriteCounter++;
		length--;
		if(buffCtrlBlock->bufferWriteCounter == buffSize)
			buffCtrlBlock->bufferWriteCounter = 0;
		if(buffCtrlBlock->bufferWriteCounter == buffCtrlBlock->bufferReadCounter)
			buffCtrlBlock->isBufferFull = true;
	}

	if((buffCtrlBlock->isBufferFull) && (length != 0))
		return BUFF_CTRL_ERR_BYTES_ABORTED;

	return BUFF_CTRL_ERR_SUCCESS;
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
