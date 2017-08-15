/*
MIT License

Copyright (c) 2016 D. Nesvera

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef NRF24L01_H
#define NRF24L01_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_spi.h"

/**
 * @brief  Transmission status enumeration
 */
typedef enum {
	NRF24L01_Transmit_Status_Lost = 0x00,   /*!< Message is lost, reached maximum number of retransmissions */
	NRF24L01_Transmit_Status_Ok = 0x01,     /*!< Message sent successfully */
	NRF24L01_Transmit_Status_Sending = 0xFF /*!< Message is still sending */
} NRF24L01_Transmit_Status_t;

/**
 * @brief  Data rate enumeration
 */
typedef enum {
	NRF24L01_DataRate_2M,  /*!< Data rate set to 2Mbps */
	NRF24L01_DataRate_1M,  /*!< Data rate set to 1Mbps */
	NRF24L01_DataRate_250k /*!< Data rate set to 250kbps */
} NRF24L01_DataRate_t;

/**
 * @brief  Output power enumeration
 */
typedef enum {
	NRF24L01_OutputPower_M18dBm,		/*!< Output power set to -18dBm */
	NRF24L01_OutputPower_M12dBm, 		/*!< Output power set to -12dBm */
	NRF24L01_OutputPower_M6dBm,  		/*!< Output power set to -6dBm */
	NRF24L01_OutputPower_0dBm    		/*!< Output power set to 0dBm */
} NRF24L01_OutputPower_t;

/**
 * @brief  CRC encoding scheme enumeration

 */
typedef enum{
	NRF24L01_CRC_Disable = 0,
	NRF24L01_CRC_8,
	NRF24L01_CRC_16
} NRF24L01_CrcLength_t;

/**
 * @brief  Struct of the NRF24L01 pins
 */
typedef struct {
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;	
} PIN_t;

typedef struct {
	PIN_t CE;
	PIN_t CSN;
	PIN_t SCK;
	PIN_t MOSI;
	PIN_t MISO;
	PIN_t IRQ;
} NRF24L01_Pins_t;

/* Gets interrupt status from device */
#define NRF24L01_GET_INTERRUPTS     TM_NRF24L01_GetStatus()

/* Interrupt masks */
#define NRF24L01_IRQ_DATA_READY     0x40 /*!< Data ready for receive */
#define NRF24L01_IRQ_TRAN_OK        0x20 /*!< Transmission went OK */
#define NRF24L01_IRQ_MAX_RT         0x10 /*!< Max retransmissions reached, last transmission failed */


/**
 * @brief	Assert the CE pin
*/
void NRF24L01_CE_LOW(void);

/**
 * @brief	Assert the CE pin
*/
void NRF24L01_CE_HIGH(void);

/**
 * @brief De-assert the CSN pin
*/
void NRF24L01_CSN_LOW(void);

/**
 * @brief Assert the CSN pin
*/
void NRF24L01_CSN_HIGH(void);

/**
 * @brief	Assert bits [6] [5] [4] to clear this bits of STATUS register
 */
void NRF24L01_ClearInterrupts(void);

/**
 * @brief  Flush TX FIFO, used in TX mode
*/
void NRF24L01_FlushTx(void);

/**
 * @brief	Flush RX FIFO, used in RX mode.
 * @note 	Should not be executed during transmission of acknowledge, that is, acknowledge package
					will not be completed.
*/
void NRF24L01_FlushRx(void);

/**
 * @brief  Initializes NRF24L01 module
 * @param  channel: channel you will use for communication, from 0 to 125 eg. working frequency from 2.4 to 2.525 GHz
 * @param  payload_size: maximum data to be sent in one packet from one NRF to another.
 * @note   Maximal payload size is 32bytes
 * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
 *                the configuration information for SPI module.
 * @retval 1
 */
uint8_t NRF24L01_Init(uint8_t channel, uint8_t payload_size, SPI_HandleTypeDef *hspi, NRF24L01_Pins_t pins );

/**
 * @brief  Sets own address. This is used for settings own id when communication with other modules
 * @note   "Own" address of one device must be the same as "TX" address of other device (and vice versa),
 *         if you want to get successful communication
 * @param  *adr: Pointer to 5-bytes length array with address
 * @retval None
 */
void NRF24L01_SetRxAddress(uint8_t* adr);

/**
 * @brief  Sets address you will communicate with
 * @note   "Own" address of one device must be the same as "TX" address of other device (and vice versa),
 *         if you want to get successful communication
 * @param  *adr: Pointer to 5-bytes length array with address
 * @retval None
 */
void NRF24L01_SetTxAddress(uint8_t* adr);

/**
 * @brief  Gets number of retransmissions needed in last transmission
 * @param  None
 * @retval Number of retransmissions, between 0 and 15.
 */
uint8_t NRF24L01_GetRetransmissionsCount(void);

/**
 * @brief  Sets NRF24L01+ to TX mode
 * @note   In this mode is NRF able to send data to another NRF module
 * @param  None
 * @retval None
 */
void NRF24L01_PowerUpTx(void);

/**
 * @brief  Sets NRF24L01+ to RX mode
 * @note   In this mode is NRF able to receive data from another NRF module.
 *         This is default mode and should be used all the time, except when sending data
 * @param  None
 * @retval None
 */
void NRF24L01_PowerUpRx(void);

/**
 * @brief  Sets NRF24L01+ to power down mode
 * @note   In power down mode, you are not able to transmit/receive data.
 *         You can wake up device using @ref TM_NRF24L01_PowerUpTx() or @ref TM_NRF24L01_PowerUpRx() functions
 *				 All register values available are maintained and th SPI is kept active.
 * @param  None
 * @retval None
 */
void NRF24L01_PowerDown(void);

/**
 * @brief  Leave low-power mode
 * @param  None
 * @retval None
 */
void NRF24L01_PowerUP(void);

/**
 * @brief  Gets transmissions status
 * @param  None
 * @retval Transmission status. Return is based on @ref TM_NRF24L01_Transmit_Status_t enumeration
 */
NRF24L01_Transmit_Status_t NRF24L01_GetTransmissionStatus(void);

/**
 * @brief  Transmits data with NRF24L01+ to another NRF module
 * @param  *data: Pointer to 8-bit array with data.
 *         Maximum length of array can be the same as "payload_size" parameter on initialization
 * @retval This parameter can be a value of @ref NRF24L01_Transmit_Status_t enumeration
 */
NRF24L01_Transmit_Status_t NRF24L01_Transmit(const void *data);

/**
 * @brief  Checks if data is ready to be read from NRF24L01+
 * @param  None
 * @retval Data ready status:
 *            - 0: No data available for receive in bufferReturns
 *            - > 0: Data is ready to be collected
 */
uint8_t NRF24L01_DataReady(void);

/**
 * @brief  Gets data from NRF24L01+
 * @param  *data: Pointer to 8-bits array where data from NRF will be saved
 * @retval None
 */
void NRF24L01_GetData(uint8_t *data);

/**
 * @brief  Sets working channel
 * @note   Channel value is just an offset in units MHz from 2.4GHz
 *         For example, if you select channel 65, then operation frequency will be set to 2.465GHz.
 *				 The channel occupies a bandwidth of less than 1MHz at 250kbps and 1Mbps and a bandwidth
 *				 of less than 2MHz at 2Mbps.
 * @param  channel: RF channel where device will operate between 0 and 125
 * @retval None 
 */
void NRF24L01_SetChannel(uint8_t channel);

/**
 * @brief  Sets RF parameters for NRF24L01+
 * @param  DataRate: Data rate selection for NRF module. This parameter can be a value of @ref TM_NRF24L01_DataRate_t enumeration
 * @param  OutPwr: Output power selection for NRF module. This parameter can be a value of @ref TM_NRF24L01_OutputPower_t enumeration
 * @retval None
 */
void NRF24L01_SetRF( NRF24L01_DataRate_t DataRate, NRF24L01_OutputPower_t OutPwr);

/**
 * @brief  Sets Power Amplifier(PA) level to one of four levels.
 * @param  OutPwr: Output power selection for NRF module. This parameter can be a value of @ref TM_NRF24L01_OutputPower_t enumeration
 * @retval None
 */
void NRF24L01_SetPaLevel( NRF24L01_OutputPower_t OutPwr );

/**
 * @brief  Gets the current Power Amplifier(PA) level.
 * @retval Returns a value from the NRF24L01_OutputPower_t enum describing the current PA setting.
 */
NRF24L01_OutputPower_t NRF24L01_GetPaLevel( void );

/**
 * @brief  Set the transmission data rate
 * @param  DataRate: Data rate selection for NRF module. This parameter can be a value of @ref TM_NRF24L01_DataRate_t enumeration
 */
void NRF24L01_SetDataRate( NRF24L01_DataRate_t DataRate );

/**
 * @brief  Gets the current transmission data rate.
 * @retval Returns a value from the NRF24L01_DataRate_t enum describing the current data rate.
 */
NRF24L01_DataRate_t NRF24L01_GetDataRate( void );

/**
 * @brief  Gets NRLF+ status register value
 * @param  None
 * @retval Status register from NRF
 */
uint8_t NRF24L01_GetStatus(void);

/**
 * @brief  Gets NRLF+ status register value
 * @param  Length: length for 8bits, 16bits or disable CRC
 * @retval None
 */
void NRF24L01_SetCrcLength( NRF24L01_CrcLength_t length );

// void NRF24L01_getCrcLength( NRF24L01_CrcLength_t length );

/**
 * @brief  Test whether there are bytes available to be read
 * @note	 Use this version to discover on which pipe the message arrived.
 * @retval true(0x01) if there is a payload available, false(0x00) if there is no payload
 */
uint8_t NRF24L01_Available( uint8_t* pipe_num );

/**
 * @brief  Enable or disable auto-acknowledgment function
 * @note	 This is enabled by default, so it's only needed if you want to turn it off for some reason.
 * @param	 pipe: which pipeline you want to modify - 0xFF disable all pipes
 * @param  State: 0x00 = disable 	0x01 = enable
 * @retval None
 */
void NRF24L01_SetAutoAck( uint8_t pipe, uint8_t state );

/**
 * @brief  Test whether there was a carrier on the line for the previous listening period
 * @note	 Useful to check for interference on the current channel.
 * @retval true(0x01) if received power levels above -64dBm, false(0x00) if received power is less than -64dBm.
 */
uint8_t NRF24L01_TestCarrier(void);

/**
 * @brief  Open a pipe for writing - transmission
 * @note	 Only one pipe can be open at once, but you can change the pipe you will listen to. Do not call
 * 				 call this this while actively listening, call stopListening() fist.
 * @param  Address: 
 * @retval None
 */
void NRF24L01_OpenWritingPipe( uint8_t* address );

/**
 * @brief  Open a pipe for reading - receive
 * @note	 Up to 6 pipes can be open for reading at once.  Open all the
 *  			 reading pipes, and then call startListening().
 * @param  Number: which pipe to open, 0-5
 * @param  Address: the 40bits address of the pipe to open
 * @retval None
 */
void NRF24L01_OpenReadingPipe( uint8_t number, uint8_t* address );

/**
 * @brief  Start listening on the pipes opened for reading. - Go to Rx mode
 * @note	 Be sure to call openReadingPipe() first.  Do not call write() while
   * 			 in this mode, without first calling stopListening().  Call
   * 			 isAvailable() to check for incoming traffic, and read() to get it.
 * @retval None
 */
void NRF24L01_StartListening(void);

/**
 * @brief  Stop listening for incoming messages -  return to standby-I
 * @note	 Do this before calling write().
 * @retval None
 */
void NRF24L01_StopListening(void);

/**
 * @brief  Read the payload
 * @note	 Return the last payload received
 * @param  Data: pointer to a buffer where the data should be written
 * @retval None
 */
uint8_t NRF24L01_Read(void* data);

/**
 * @brief  Get Static payload size
 * @retval Number of bytes in the payload
 */
uint8_t NRF24L01_GetPayloadSize(void);

/**
 * @brief  Get Dynamic payload size
 * @retval Payload size of last received dynamic payload
 */
uint8_t NRF24L01_GetDynamicPayloadSize(void);

/**
 * @brief  Set the number and delay of retries upon failed submit
 * @param  delay How long to wait between each retry, in multiples of 250us,
 * max is 15.  0 means 250us, 15 means 4000us.
 * @param  count How many retries before giving up, max 15
 * @retval NONE
 */
void NRF24L01_SetRetries(uint8_t delay, uint8_t count);


#endif /* __NRF24L01_H */
