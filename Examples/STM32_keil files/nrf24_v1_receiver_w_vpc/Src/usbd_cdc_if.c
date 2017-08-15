/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @brief          :
  ******************************************************************************
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  * 1. Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  * this list of conditions and the following disclaimer in the documentation
  * and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of its contributors
  * may be used to endorse or promote products derived from this software
  * without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"

/* USER CODE BEGIN INCLUDE */

#include "string.h"


// Par�metros usados para definir a comunica��o serial
USBD_CDC_LineCodingTypeDef LineCoding = {
	115200,		/* Baud rate 				*/
	0x00,			/* Stop bits = 1 		*/
	0x00,			/* Parity = none		*/
	0x08			/* num of bits = 8	*/
};

/* USB handler declaration */
/* Handle for USB Full Speed IP */
USBD_HandleTypeDef  *hUsbDevice_0;
/* USER CODE BEGIN PRIVATE_VARIABLES */

uint32_t UserTxBufPtrIn = 0;
uint32_t UserTxBufPtrOut = 0;

/* Received Data over USB are stored in this buffer       */
uint8_t UserRxBufferFS[1024];

/* Send Data over USB CDC are stored in this buffer       */
uint8_t UserTxBufferFS[1024];
	
extern uint8_t stringRecebida;

extern USBD_HandleTypeDef hUsbDeviceFS;

/**
  * @}
  */ 
  
/** @defgroup USBD_CDC_Private_FunctionPrototypes
  * @{
  */
static int8_t CDC_Init_FS     (void);
static int8_t CDC_DeInit_FS   (void);
static int8_t CDC_Control_FS  (uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS  (uint8_t* pbuf, uint32_t *Len);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */ 
  
USBD_CDC_ItfTypeDef USBD_Interface_fops_FS = 
{
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,  
  CDC_Receive_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  CDC_Init_FS
  *         Initializes the CDC media low layer over the FS USB IP
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_FS(void)
{
  hUsbDevice_0 = &hUsbDeviceFS;
  /* USER CODE BEGIN 3 */ 
  /* Set Application Buffers */
  USBD_CDC_SetTxBuffer(hUsbDevice_0, UserTxBufferFS, 0);
  USBD_CDC_SetRxBuffer(hUsbDevice_0, UserRxBufferFS);
  return (USBD_OK);
  /* USER CODE END 3 */ 
}

/**
  * @brief  CDC_DeInit_FS
  *         DeInitializes the CDC media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
  /* USER CODE BEGIN 4 */ 
  return (USBD_OK);
  /* USER CODE END 4 */ 
}

/**
  * @brief  CDC_Control_FS
  *         Manage the CDC class requests
  * @param  cmd: Command code            
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_FS  (uint8_t cmd, uint8_t* pbuf, uint16_t length)
{ 
  /* USER CODE BEGIN 5 */
  switch (cmd)
  {
  case CDC_SEND_ENCAPSULATED_COMMAND:
 
    break;

  case CDC_GET_ENCAPSULATED_RESPONSE:
 
    break;

  case CDC_SET_COMM_FEATURE:
 
    break;

  case CDC_GET_COMM_FEATURE:

    break;

  case CDC_CLEAR_COMM_FEATURE:

    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */ 
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
  case CDC_SET_LINE_CODING:   
		pbuf[0] = (uint8_t)(LineCoding.bitrate);
		pbuf[1] = (uint8_t)(LineCoding.bitrate >> 8 );
		pbuf[2] = (uint8_t)(LineCoding.bitrate >> 16 );
		pbuf[3] = (uint8_t)(LineCoding.bitrate >> 24 );
		pbuf[4] = LineCoding.format;
		pbuf[5] = LineCoding.paritytype;
		pbuf[6] = LineCoding.datatype;
    break;

  case CDC_GET_LINE_CODING:     

    break;

  case CDC_SET_CONTROL_LINE_STATE:

    break;

  case CDC_SEND_BREAK:
 
    break;    
    
  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  CDC_Receive_FS
  *         Data received over USB OUT endpoint are sent over CDC interface 
  *         through this function.
  *           
  *         @note
  *         This function will block any OUT packet reception on USB endpoint 
  *         untill exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result 
  *         in receiving more data while previous ones are still not sent.
  *                 
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Receive_FS (uint8_t* Buf, uint32_t *Len)
{
	
 if( UserTxBufPtrIn + *Len <= 1024 )
	{
		memcpy( (uint8_t*)(UserTxBufferFS + UserTxBufPtrIn ), Buf, *Len );
		UserTxBufPtrIn += *Len;
	
	}
	else
	{
		memcpy( (uint8_t*)(UserTxBufferFS), Buf, *Len );
		UserTxBufPtrIn = *Len;
		UserTxBufPtrOut = 0;
	}
			
	return (USBD_OK);

}

/**
  * @brief  CDC_Transmit_FS
  *         Data send over USB IN endpoint are sent over CDC interface 
  *         through this function.           
  *         @note
  *         
  *                 
  * @param  Buf: Buffer of data to be send
  * @param  Len: Number of data to be send (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
	
	if( !hUsbDevice_0 ){
		return 0;
	}
	
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDevice_0->pClassData;
  if (hcdc->TxState != 0){
    return USBD_BUSY;
  }
  USBD_CDC_SetTxBuffer(hUsbDevice_0, Buf, Len);
  result = USBD_CDC_TransmitPacket(hUsbDevice_0);
  return result;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

