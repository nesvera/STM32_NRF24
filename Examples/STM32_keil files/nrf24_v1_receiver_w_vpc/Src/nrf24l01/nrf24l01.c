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

#include "nrf24l01.h"
#include "stm32f1xx_hal.h"

// APAGAR BUG REVER



// colocar os copyright
// os dataseet do nrf link
// os datasheet do stm link
// based

/* NRF24L01+ registers*/
#define NRF24L01_REG_CONFIG				0x00	//Configuration Register
#define NRF24L01_REG_EN_AA				0x01	//Enable ‘Auto Acknowledgment’ Function
#define NRF24L01_REG_EN_RXADDR		0x02	//Enabled RX Addresses
#define NRF24L01_REG_SETUP_AW			0x03	//Setup of Address Widths (common for all data pipes)
#define NRF24L01_REG_SETUP_RETR		0x04	//Setup of Automatic Retransmission
#define NRF24L01_REG_RF_CH				0x05	//RF Channel
#define NRF24L01_REG_RF_SETUP			0x06	//RF Setup Register	
#define NRF24L01_REG_STATUS				0x07	//Status Register
#define NRF24L01_REG_OBSERVE_TX		0x08	//Transmit observe register
#define NRF24L01_REG_RPD					0x09	
#define NRF24L01_REG_RX_ADDR_P0		0x0A	//Receive address data pipe 0. 5 Bytes maximum length.
#define NRF24L01_REG_RX_ADDR_P1		0x0B	//Receive address data pipe 1. 5 Bytes maximum length.
#define NRF24L01_REG_RX_ADDR_P2		0x0C	//Receive address data pipe 2. Only LSB
#define NRF24L01_REG_RX_ADDR_P3		0x0D	//Receive address data pipe 3. Only LSB
#define NRF24L01_REG_RX_ADDR_P4		0x0E	//Receive address data pipe 4. Only LSB
#define NRF24L01_REG_RX_ADDR_P5		0x0F	//Receive address data pipe 5. Only LSB
#define NRF24L01_REG_TX_ADDR			0x10	//Transmit address. Used for a PTX device only
#define NRF24L01_REG_RX_PW_P0			0x11	
#define NRF24L01_REG_RX_PW_P1			0x12	
#define NRF24L01_REG_RX_PW_P2			0x13	
#define NRF24L01_REG_RX_PW_P3			0x14	
#define NRF24L01_REG_RX_PW_P4			0x15	
#define NRF24L01_REG_RX_PW_P5			0x16	
#define NRF24L01_REG_FIFO_STATUS	0x17	//FIFO Status Register
#define NRF24L01_REG_DYNPD				0x1C	//Enable dynamic payload length
#define NRF24L01_REG_FEATURE			0x1D

/* Registers default values */
#define NRF24L01_REG_DEFAULT_VAL_CONFIG				0x08	
#define NRF24L01_REG_DEFAULT_VAL_EN_AA				0x3F	
#define NRF24L01_REG_DEFAULT_VAL_EN_RXADDR		0x03	
#define NRF24L01_REG_DEFAULT_VAL_SETUP_AW			0x03	
#define NRF24L01_REG_DEFAULT_VAL_SETUP_RETR		0x03	
#define NRF24L01_REG_DEFAULT_VAL_RF_CH				0x02	
#define NRF24L01_REG_DEFAULT_VAL_RF_SETUP			0x0E	
#define NRF24L01_REG_DEFAULT_VAL_STATUS				0x0E	
#define NRF24L01_REG_DEFAULT_VAL_OBSERVE_TX		0x00	
#define NRF24L01_REG_DEFAULT_VAL_RPD					0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_0	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_1	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_2	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_3	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_4	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_0	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_1	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_2	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_3	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_4	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P2		0xC3	
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P3		0xC4	
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P4		0xC5
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P5		0xC6
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_0		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_1		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_2		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_3		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_4		0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P0			0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P1			0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P2			0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P3			0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P4			0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P5			0x00
#define NRF24L01_REG_DEFAULT_VAL_FIFO_STATUS	0x17		//0x11 do codigo original
#define NRF24L01_REG_DEFAULT_VAL_DYNPD				0x00
#define NRF24L01_REG_DEFAULT_VAL_FEATURE			0x00

/* Commands */
#define NRF24L01_CMD_R_REGISTER          0x00
#define NRF24L01_CMD_W_REGISTER          0x20
#define NRF24L01_CMD_R_RX_PAYLOAD        0x61
#define NRF24L01_CMD_W_TX_PAYLOAD        0xA0
#define NRF24L01_CMD_FLUSH_TX            0xE1
#define NRF24L01_CMD_FLUSH_RX            0xE2
#define NRF24L01_CMD_REUSE_TX_PL         0xE3
#define NRF24L01_CMD_ACTIVATE            0x50
#define NRF24L01_CMD_R_RX_PL_WID         0x60
#define NRF24L01_CMD_W_ACK_PAYLOAD       0xA8
#define NRF24L01_CMD_W_TX_PAYLOAD_NOACK  0xB0
#define NRF24L01_CMD_NOP                 0xFF
 
#define NRF24L01_SPI_TIMEOUT 100
#define NRF24L01_TX_TIMEOUT 500
#define NRF24L01_RX_TIMEOUT 500

/* Configuration register*/
#define NRF24L01_MASK_RX_DR		6
#define NRF24L01_MASK_TX_DS		5
#define NRF24L01_MASK_MAX_RT	4
#define NRF24L01_EN_CRC				3
#define NRF24L01_CRCO					2
#define NRF24L01_PWR_UP				1
#define NRF24L01_PRIM_RX			0

/* RX/TX control */
#define PRX		1		// primary receiver mode
#define PTX		0		// primary transmitter mode

/* Enable auto acknowledgment*/
#define NRF24L01_ENAA_P5		5
#define NRF24L01_ENAA_P4		4
#define NRF24L01_ENAA_P3		3
#define NRF24L01_ENAA_P2		2
#define NRF24L01_ENAA_P1		1
#define NRF24L01_ENAA_P0		0

/* Enable rx addresses */
#define NRF24L01_ERX_P5			5
#define NRF24L01_ERX_P4			4
#define NRF24L01_ERX_P3			3
#define NRF24L01_ERX_P2			2
#define NRF24L01_ERX_P1			1
#define NRF24L01_ERX_P0			0

/* Setup of address width */
#define NRF24L01_AW				0 //2 bits

/* Setup of auto re-transmission*/
#define NRF24L01_ARD			4 //4 bits
#define NRF24L01_ARC			0 //4 bits

/* RF setup register*/
#define NRF24L01_PLL_LOCK			4
#define NRF24L01_RF_DR_LOW		5
#define NRF24L01_RF_DR_HIGH		3
#define NRF24L01_RF_DR				3
#define NRF24L01_RF_PWR				1 //2 bits   

/* General status register */
#define NRF24L01_RX_DR			6
#define NRF24L01_TX_DS			5
#define NRF24L01_MAX_RT			4
#define NRF24L01_RX_P_NO		1 //3 bits
#define NRF24L01_TX_FULL		0

/* Transmit observe register */
#define NRF24L01_PLOS_CNT		4 //4 bits
#define NRF24L01_ARC_CNT		0 //4 bits

/* FIFO status*/
#define NRF24L01_TX_REUSE		6
#define NRF24L01_FIFO_FULL	5
#define NRF24L01_TX_EMPTY		4
#define NRF24L01_RX_FULL		1
#define NRF24L01_RX_EMPTY		0

//Dynamic length
#define NRF24L01_DPL_P0			0
#define NRF24L01_DPL_P1			1
#define NRF24L01_DPL_P2			2
#define NRF24L01_DPL_P3			3
#define NRF24L01_DPL_P4			4
#define NRF24L01_DPL_P5			5

/* Transmitter power*/
#define NRF24L01_M18DBM			0 //-18 dBm
#define NRF24L01_M12DBM			1 //-12 dBm
#define NRF24L01_M6DBM			2 //-6 dBm
#define NRF24L01_0DBM				3 //0 dBm

/* Data rates */
#define NRF24L01_2MBPS			0
#define NRF24L01_1MBPS			1
#define NRF24L01_250KBPS		2

/* Configuration */
#define NRF24L01_CONFIG			((1 << NRF24L01_EN_CRC) | (0 << NRF24L01_CRCO))

#define NRF24L01_TRANSMISSON_OK 			0
#define NRF24L01_MESSAGE_LOST   			1

#define NRF24L01_CHECK_BIT(value, bit)       (value & (1 << bit))

typedef struct {
	uint8_t PayloadSize;							//Payload size
	uint8_t Channel;									//Channel selected
	NRF24L01_OutputPower_t OutPwr;		//Output power
	NRF24L01_DataRate_t DataRate;			//Data rate
	SPI_HandleTypeDef	*hspi;					//hspi: pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module.
} NRF24L01_t;	

/* Private functions */
void 		NRF24L01_InitPins(void);
uint8_t NRF24L01_ReadBit(uint8_t reg, uint8_t bit);
uint8_t NRF24L01_ReadRegister(uint8_t reg);
void 		NRF24L01_ReadRegisterMulti(uint8_t reg, uint8_t* data, uint8_t length);
uint8_t NRF24L01_RxFifoEmpty(void);
void 		NRF24L01_SoftwareReset(void);
void 		NRF24L01_WriteBit(uint8_t reg, uint8_t bit, uint8_t value);
void 		NRF24L01_WriteRegister( uint8_t reg, uint8_t value );
void 		NRF24L01_WriteRegisterMulti(uint8_t reg, uint8_t *data, uint8_t length);
void		NRF24L01_SpiInit( SPI_HandleTypeDef *hspi, NRF24L01_Pins_t pins );

/* NRF structure */
static NRF24L01_t NRF24L01_Struct;

/* NRF Pins structure */
static NRF24L01_Pins_t NRF24L01_Pins;

uint8_t pipe0_reading_address[5];	// last address set on pipe 0 for reading

void NRF24L01_CE_LOW()
{
	HAL_GPIO_WritePin( NRF24L01_Pins.CE.GPIOx, NRF24L01_Pins.CE.GPIO_Pin, GPIO_PIN_RESET );
}

void NRF24L01_CE_HIGH(void)
{
	HAL_GPIO_WritePin( NRF24L01_Pins.CE.GPIOx, NRF24L01_Pins.CE.GPIO_Pin, GPIO_PIN_SET );
}

void NRF24L01_CSN_LOW(void)
{
	HAL_GPIO_WritePin( NRF24L01_Pins.CSN.GPIOx, NRF24L01_Pins.CSN.GPIO_Pin, GPIO_PIN_RESET );	
}

void NRF24L01_CSN_HIGH(void)
{
	HAL_GPIO_WritePin( NRF24L01_Pins.CSN.GPIOx, NRF24L01_Pins.CSN.GPIO_Pin, GPIO_PIN_SET );	
}

/* Clear interrupt flags */
void NRF24L01_ClearInterrupts(void)
{
	NRF24L01_WriteRegister( NRF24L01_REG_STATUS, (1<<NRF24L01_RX_DR)|(1<<NRF24L01_TX_DS)|(1<<NRF24L01_MAX_RT) );		// setting bits 4,5,6
}

/* Flush FIFOs */
void NRF24L01_FlushRx(void)
{
	NRF24L01_CSN_LOW();
		
	uint8_t aux = NRF24L01_CMD_FLUSH_RX;
	HAL_SPI_Transmit( NRF24L01_Struct.hspi, &aux, 1, NRF24L01_SPI_TIMEOUT );
	
	NRF24L01_CSN_HIGH(); 
}

void NRF24L01_FlushTx(void)
{
	NRF24L01_CSN_LOW(); 
		
	uint8_t aux = NRF24L01_CMD_FLUSH_TX;
	HAL_SPI_Transmit( NRF24L01_Struct.hspi, &aux, 1, NRF24L01_SPI_TIMEOUT );
		
	NRF24L01_CSN_HIGH(); 
}


uint8_t NRF24L01_Init(uint8_t channel, uint8_t payload_size, SPI_HandleTypeDef *hspi, NRF24L01_Pins_t pins )
{			
	NRF24L01_Pins = pins;
	
	for( int ind = 0 ; ind < 5 ; ind++ )
		pipe0_reading_address[ind] = 0;
	
	// Initialize SPI 
	NRF24L01_SpiInit( hspi, NRF24L01_Pins );
	
	// We can consider the worst settle time and use to all
	HAL_Delay(5);
	
	/* Max payload is 32bytes */
	if (payload_size > 32){
		payload_size = 32;
	}
	
	/* Fill structure */
	NRF24L01_Struct.Channel = !channel; /* Set channel to some different value for TM_NRF24L01_SetChannel() function */
	NRF24L01_Struct.PayloadSize = payload_size;
	NRF24L01_Struct.OutPwr = NRF24L01_OutputPower_0dBm;
	NRF24L01_Struct.DataRate = NRF24L01_DataRate_1M;
	NRF24L01_Struct.hspi = hspi;
	
	/* Reset nRF24L01+ to power on registers values */
	NRF24L01_SoftwareReset();
	
	/* Set 1500uS timeouts ( minimum for 32 bytes payload in 250KBPS ) */
	NRF24L01_WriteRegister( NRF24L01_REG_SETUP_RETR, ((4 << NRF24L01_ARD)|(15 << NRF24L01_ARC)) );
	
	/* Channel select */
	NRF24L01_SetChannel(channel);
	
	/* Set pipeline to the payload size */
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P0, NRF24L01_Struct.PayloadSize); 
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P1, NRF24L01_Struct.PayloadSize);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P2, NRF24L01_Struct.PayloadSize);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P3, NRF24L01_Struct.PayloadSize);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P4, NRF24L01_Struct.PayloadSize);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P5, NRF24L01_Struct.PayloadSize);
	
	/* Set RF settings (1mbps, output power) */
	NRF24L01_SetRF( NRF24L01_Struct.DataRate, NRF24L01_Struct.OutPwr);
	
	/* Config register */
	NRF24L01_WriteRegister(NRF24L01_REG_CONFIG, NRF24L01_CONFIG);
	
	/* Enable auto-acknowledgment for all pipes */
	NRF24L01_WriteRegister(NRF24L01_REG_EN_AA, 0x3F);
	
	/* Enable RX addresses */
	NRF24L01_WriteRegister(NRF24L01_REG_EN_RXADDR, 0x3F);

	/* Auto retransmit delay: 1000 (4x250) us and Up to 15 retransmit trials */
	NRF24L01_WriteRegister(NRF24L01_REG_SETUP_RETR, 0x4F);
	
	/* Dynamic length configurations: No dynamic length */
	NRF24L01_WriteRegister(NRF24L01_REG_DYNPD, (0 << NRF24L01_DPL_P0) | (0 << NRF24L01_DPL_P1) | (0 << NRF24L01_DPL_P2) | (0 << NRF24L01_DPL_P3) | (0 << NRF24L01_DPL_P4) | (0 << NRF24L01_DPL_P5));
	
	/* Initialize CRC 2bytes */
	NRF24L01_SetCrcLength( NRF24L01_CRC_16 );
	
	/* Clear FIFOs */
	NRF24L01_FlushTx();
	NRF24L01_FlushRx();
	
	/* Clear interrupts */
	NRF24L01_ClearInterrupts();
	
	/* Go to RX mode */
	NRF24L01_PowerUpRx();
	
	/* Return OK */
	return 1;
}

void NRF24L01_SpiInit( SPI_HandleTypeDef *hspi, NRF24L01_Pins_t pins )
{
	GPIO_InitTypeDef GPIO_InitStruct;
  //if(hspi->Instance==SPI1)
  //{

	hspi->Instance = SPI1;
  hspi->Init.Mode = SPI_MODE_MASTER;														
  hspi->Init.Direction = SPI_DIRECTION_2LINES;									
  hspi->Init.DataSize = SPI_DATASIZE_8BIT;											
  hspi->Init.CLKPolarity = SPI_POLARITY_LOW;										
  hspi->Init.CLKPhase = SPI_PHASE_1EDGE;												
  hspi->Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;			
  hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;												
  hspi->Init.TIMode = SPI_TIMODE_DISABLE;
  hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi->Init.CRCPolynomial = 10;																	
  HAL_SPI_Init(hspi);
	
  /**SPI1 GPIO Configuration    
  PA4     ------> SPI1_NSS
  PA5     ------> SPI1_SCK
  PA6     ------> SPI1_MISO
  PA7     ------> SPI1_MOSI `
  */
		
	GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;				// gpio alternative function
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
	/* Alterar os pinos de NSS e CE para Push-pull, pull up resistor e low speed */		
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;				// gpio alternative function
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/* CSN(SS) high = disable SPI */
	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4, GPIO_PIN_SET );

	/* CE low = disable TX/RX */
	HAL_GPIO_WritePin( GPIOB, GPIO_PIN_0, GPIO_PIN_RESET );

  //}
		
}

void NRF24L01_SoftwareReset(void)
{
	uint8_t data[5];
	
	NRF24L01_WriteRegister( NRF24L01_REG_CONFIG, 			NRF24L01_REG_DEFAULT_VAL_CONFIG);
	NRF24L01_WriteRegister( NRF24L01_REG_EN_AA,				NRF24L01_REG_DEFAULT_VAL_EN_AA);
	NRF24L01_WriteRegister( NRF24L01_REG_EN_RXADDR, 	NRF24L01_REG_DEFAULT_VAL_EN_RXADDR);
	NRF24L01_WriteRegister( NRF24L01_REG_SETUP_AW, 		NRF24L01_REG_DEFAULT_VAL_SETUP_AW);
	NRF24L01_WriteRegister( NRF24L01_REG_SETUP_RETR, 	NRF24L01_REG_DEFAULT_VAL_SETUP_RETR);
	NRF24L01_WriteRegister( NRF24L01_REG_RF_CH, 			NRF24L01_REG_DEFAULT_VAL_RF_CH);
	NRF24L01_WriteRegister( NRF24L01_REG_RF_SETUP, 		NRF24L01_REG_DEFAULT_VAL_RF_SETUP);
	NRF24L01_WriteRegister( NRF24L01_REG_STATUS, 			NRF24L01_REG_DEFAULT_VAL_STATUS);
	NRF24L01_WriteRegister( NRF24L01_REG_OBSERVE_TX, 	NRF24L01_REG_DEFAULT_VAL_OBSERVE_TX);
	NRF24L01_WriteRegister( NRF24L01_REG_RPD, 				NRF24L01_REG_DEFAULT_VAL_RPD);
	
	//P0
	data[0] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_0;
	data[1] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_1;
	data[2] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_2;
	data[3] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_3;
	data[4] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_4;
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P0, data, 5);
	
	//P1
	data[0] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_0;
	data[1] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_1;
	data[2] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_2;
	data[3] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_3;
	data[4] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_4;
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P1, data, 5);
	
	NRF24L01_WriteRegister(NRF24L01_REG_RX_ADDR_P2, 	NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P2);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_ADDR_P3, 	NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P3);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_ADDR_P4, 	NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P4);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_ADDR_P5, 	NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P5);
	
	//TX
	data[0] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_0;
	data[1] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_1;
	data[2] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_2;
	data[3] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_3;
	data[4] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_4;
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_TX_ADDR, data, 5);
	
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P0, 		NRF24L01_REG_DEFAULT_VAL_RX_PW_P0);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P1, 		NRF24L01_REG_DEFAULT_VAL_RX_PW_P1);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P2, 		NRF24L01_REG_DEFAULT_VAL_RX_PW_P2);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P3, 		NRF24L01_REG_DEFAULT_VAL_RX_PW_P3);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P4, 		NRF24L01_REG_DEFAULT_VAL_RX_PW_P4);
	NRF24L01_WriteRegister(NRF24L01_REG_RX_PW_P5, 		NRF24L01_REG_DEFAULT_VAL_RX_PW_P5);
	NRF24L01_WriteRegister(NRF24L01_REG_FIFO_STATUS, 	NRF24L01_REG_DEFAULT_VAL_FIFO_STATUS);
	NRF24L01_WriteRegister(NRF24L01_REG_DYNPD, 				NRF24L01_REG_DEFAULT_VAL_DYNPD);
	NRF24L01_WriteRegister(NRF24L01_REG_FEATURE, 			NRF24L01_REG_DEFAULT_VAL_FEATURE);
}

/**
 * @brief Read a bit of the current register value
 * @param	reg  
 * @param	bit
 * @param value
 */
uint8_t NRF24L01_ReadBit(uint8_t reg, uint8_t bit)															
{
	uint8_t tmp;
	tmp = NRF24L01_ReadRegister(reg);
	
	if (!NRF24L01_CHECK_BIT(tmp, bit)){
		return 0;
	}
	return 1;
}

/**
 * @brief Change a bit of the current register value
 * @param	reg  
 * @param	bit
 * @param value
 */
void NRF24L01_WriteBit(uint8_t reg, uint8_t bit, uint8_t value)											
{
	uint8_t tmp;
	/* Read register */
	tmp = NRF24L01_ReadRegister(reg);
	
	/* Make operation */
	if (value){
		tmp |= 1 << bit;
	}else{
		tmp &= ~(1 << bit);
	}
	/* Write back */
	NRF24L01_WriteRegister(reg, tmp);
}

uint8_t NRF24L01_ReadRegister(uint8_t reg) 																															
{	
	/* NRF24L01 shifts the value of status register at the first MOSI byte and at the second 
	byte the value of the interested register */
	
	// Initialize buffers
	uint8_t txBuffer[2];
	txBuffer[0] = NRF24L01_CMD_R_REGISTER | reg;					// REGISTER | READ_MASK(000A AAAA)
	txBuffer[1] = NRF24L01_CMD_NOP;												// REVER -  problema se não inicializar?
	
	uint8_t rxBuffer[2] = { 0x00, 0x00 };
	
	NRF24L01_CSN_LOW();
	
	// Transmit the txBuffer and receive data on rxBuffer at the time
	HAL_SPI_TransmitReceive( NRF24L01_Struct.hspi, txBuffer, rxBuffer, 2, NRF24L01_SPI_TIMEOUT );
	
	NRF24L01_CSN_HIGH();
	
	return rxBuffer[1];
}

void NRF24L01_WriteRegister( uint8_t reg, uint8_t value ) 																					
{
	uint8_t txBuffer[ 2 ];
	txBuffer[0] = NRF24L01_CMD_W_REGISTER | reg;					// REGISTER | WRITE_MASK(001A AAAA)
	txBuffer[1] = value;																	// DATA to write
		
	NRF24L01_CSN_LOW();
	
	// Just transmit
	HAL_SPI_Transmit( NRF24L01_Struct.hspi, txBuffer, 2, NRF24L01_SPI_TIMEOUT ); 
	
	NRF24L01_CSN_HIGH();
}

// REVER qual byte vai primeiro (LSBYTE ou MSBYTE)
void NRF24L01_ReadRegisterMulti(uint8_t reg, uint8_t* data, uint8_t length) 
{
	uint8_t txBuffer[ length + 1 ];
	uint8_t rxBuffer[ length + 1 ];

	txBuffer[0] = NRF24L01_CMD_R_REGISTER | reg;									
	for( uint8_t ind = 1 ; ind <= length ; ind++ ){					// (APAGAR) melhorar otimizacao, talvez de bug por nao inicializar
		txBuffer[ind] = NRF24L01_CMD_NOP;
	}
		
	NRF24L01_CSN_LOW();
	
	HAL_SPI_TransmitReceive( NRF24L01_Struct.hspi, txBuffer, rxBuffer, (length+1), NRF24L01_SPI_TIMEOUT );
	
	NRF24L01_CSN_HIGH();

	for( uint8_t ind = 0 ; ind < length ; ind++ ){
		data[ind] = rxBuffer[ ind + 1 ]; 
	}
}

// REVER qual byte vai primeiro (LSBYTE ou MSBYTE)
void NRF24L01_WriteRegisterMulti(uint8_t reg, uint8_t *data, uint8_t length) // REVER
{
	uint8_t txBuffer[ length + 1 ];
	
	txBuffer[0] = NRF24L01_CMD_W_REGISTER | reg;
	for( uint8_t ind = 0 ; ind < length ; ind++ ){
		txBuffer[ ind + 1 ] = data[ind];
	}
	
	NRF24L01_CSN_LOW();
	
	HAL_SPI_Transmit( NRF24L01_Struct.hspi, txBuffer, (length+1), NRF24L01_SPI_TIMEOUT );
	
	NRF24L01_CSN_HIGH();	
}

void NRF24L01_SetRxAddress(uint8_t *adr)
{
	NRF24L01_CE_LOW();
	
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P1, adr, 5);
	
	NRF24L01_CE_HIGH();
}

void NRF24L01_SetTxAddress(uint8_t *adr)
{
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P0, adr, 5);
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_TX_ADDR, adr, 5);
}

void NRF24L01_PowerDown(void)
{																																										
	NRF24L01_CE_LOW();
	NRF24L01_WriteBit(NRF24L01_REG_CONFIG, NRF24L01_PWR_UP, RESET );
}

void NRF24L01_PowerUP(void)
{										
	NRF24L01_WriteBit(NRF24L01_REG_CONFIG, NRF24L01_PWR_UP, SET );
} 

void NRF24L01_PowerUpRx(void)
{
	/* Disable RX/TX mode */
	NRF24L01_CE_LOW();
	/* Clear RX buffer */
	NRF24L01_FlushRx();
	/* Clear interrupts */
	NRF24L01_ClearInterrupts();
	/* Setup RX mode */
	NRF24L01_WriteRegister(NRF24L01_REG_CONFIG, NRF24L01_CONFIG | 1 << NRF24L01_PWR_UP | 1 << NRF24L01_PRIM_RX );
	/* Start listening */
	NRF24L01_CE_HIGH();
}

void NRF24L01_PowerUpTx(void)
{
	NRF24L01_ClearInterrupts();
	NRF24L01_WriteRegister(NRF24L01_REG_CONFIG, (NRF24L01_ReadRegister( NRF24L01_REG_CONFIG ) & ~(1 << NRF24L01_PRIM_RX)) | NRF24L01_CONFIG | (1 << NRF24L01_PWR_UP));				// PWR_UP = 1, PRIM_RX = 0	
}

// REVER - const void
NRF24L01_Transmit_Status_t NRF24L01_Transmit(const void *data)
{
	const uint8_t* aux;
	aux = data;
	
	uint8_t length = NRF24L01_Struct.PayloadSize;

	/* Chip enable put to low, disable it */
	NRF24L01_CE_LOW();
		
	/* Clear TX FIFO from NRF24L01+ */
	NRF24L01_FlushTx();	
	
	uint8_t txBuffer[length+1];
	
	/* Send write payload command */
	txBuffer[0] = NRF24L01_CMD_W_TX_PAYLOAD;
	/* Fill payload with data*/
	for( int ind = 0 ; ind < length ; ind++ ){				// REVER se precisa iniciar
		txBuffer[ind+1] = aux[ind];
	}
	
	/* Go to power up tx mode */
	NRF24L01_PowerUpTx();
	
	/* Settling time = 150us */
	HAL_Delay(1);
	
	
	/* Send payload to nRF24L01+ */
	NRF24L01_CSN_LOW();
	
	/* Fill payload with data*/
	HAL_SPI_Transmit( NRF24L01_Struct.hspi, txBuffer, (length+1), NRF24L01_SPI_TIMEOUT );
	
	/* Disable SPI */
	NRF24L01_CSN_HIGH();
		
	/* Send data! */
	NRF24L01_CE_HIGH();
	HAL_Delay(1);
	NRF24L01_CE_LOW();
	
	/* From J. Coliz
	
	At this point we could return from a non-blocking write, an then call the rest after an interrupt.
	Instead, we are going to block here until we get TX_DS(transmission completed and ack'd) or MAX_RT
	(maximum retries, transmission failed). Also, we'll timeout in case the radio is flaky and we get
	neither.
	In the end, the send should be blocking. It comes back in 60ms worst case.
	
	*/
		
	NRF24L01_Transmit_Status_t status = NRF24L01_Transmit_Status_Lost;
	uint32_t sent_at = HAL_GetTick();
	const uint32_t timeout = NRF24L01_TX_TIMEOUT;
	
	do{
		status =  NRF24L01_GetTransmissionStatus();
		
	}while( (HAL_GetTick() - sent_at < timeout) && (status != NRF24L01_Transmit_Status_Ok)  );
	
	NRF24L01_PowerDown();
	
	NRF24L01_FlushTx();
	
	
	
	
	return status;
}

void NRF24L01_GetData(uint8_t* data)																															// <<<<<--------------------------------
{
	/* Pull down chip select */
	NRF24L01_CSN_LOW();
	
	/* Send read payload command*/
	uint8_t aux = NRF24L01_CMD_R_RX_PAYLOAD;
	HAL_SPI_Transmit( NRF24L01_Struct.hspi, &aux, 1, NRF24L01_SPI_TIMEOUT );
	
	/* Read payload */
	HAL_SPI_TransmitReceive( NRF24L01_Struct.hspi, data, data, NRF24L01_Struct.PayloadSize, NRF24L01_SPI_TIMEOUT );
	
	/* Pull up chip select */
	NRF24L01_CSN_HIGH();
	
	/* Reset status register, clear RX_DR interrupt flag */
	NRF24L01_WriteRegister(NRF24L01_REG_STATUS, (1 << NRF24L01_RX_DR));
}

uint8_t NRF24L01_DataReady(void)
{
	uint8_t status = NRF24L01_GetStatus();
	
	if (NRF24L01_CHECK_BIT(status, NRF24L01_RX_DR)) {
		return 1;
	}
	return !NRF24L01_RxFifoEmpty();
}

uint8_t NRF24L01_RxFifoEmpty(void)
{
	uint8_t reg = NRF24L01_ReadRegister(NRF24L01_REG_FIFO_STATUS);
	return NRF24L01_CHECK_BIT(reg, NRF24L01_RX_EMPTY);
}

// funcao para tx_full
// funcao para tx_empty
// funcao para rx_full

uint8_t NRF24L01_GetStatus(void)
{
	uint8_t status;
	
	NRF24L01_CSN_LOW();
	
	/* First received byte is always status register */
	uint8_t aux  = NRF24L01_CMD_NOP;
	HAL_SPI_TransmitReceive( NRF24L01_Struct.hspi, &aux, &status, 1, NRF24L01_SPI_TIMEOUT );
	
	/* Pull up chip select */
	NRF24L01_CSN_HIGH();
	
	return status;
}

NRF24L01_Transmit_Status_t NRF24L01_GetTransmissionStatus(void)
{
	uint8_t status = NRF24L01_GetStatus();
	if (NRF24L01_CHECK_BIT(status, NRF24L01_TX_DS)) {
		/* Successfully sent */
		return NRF24L01_Transmit_Status_Ok;
	} else if (NRF24L01_CHECK_BIT(status, NRF24L01_MAX_RT)) {
		/* Message lost */
		return NRF24L01_Transmit_Status_Lost;
	}
	
	/* Still sending */
	return NRF24L01_Transmit_Status_Sending;
}

uint8_t NRF24L01_GetRetransmissionsCount(void)
{
	/* Low 4 bits */
	return NRF24L01_ReadRegister(NRF24L01_REG_OBSERVE_TX) & 0x0F;
}

void NRF24L01_SetChannel(uint8_t channel) 																										
{
	if(channel <= 125 && channel != NRF24L01_Struct.Channel){
		/* Store new channel setting */
		NRF24L01_Struct.Channel = channel;
		/* Write channel */
		NRF24L01_WriteRegister(NRF24L01_REG_RF_CH, channel);
	}
}

void NRF24L01_SetCrcLength( NRF24L01_CrcLength_t length )
{
	uint8_t config = NRF24L01_ReadRegister( NRF24L01_REG_CONFIG );
	
	// "Switch uses RAM (evil!)" - J. Coliz 
	if( length == NRF24L01_CRC_Disable ){
		config &= ~( (1<< NRF24L01_CRCO) | (1<<NRF24L01_EN_CRC) );
			
	// crc = 1byte
	}else if( length == NRF24L01_CRC_8 ){
		config |= (1<<NRF24L01_EN_CRC);
		
	// crc = 2bytes
	}else{
		config |= (1<<NRF24L01_EN_CRC)|(1<<NRF24L01_CRCO);
	}
	
	NRF24L01_WriteRegister( NRF24L01_REG_CONFIG, config );
}

void NRF24L01_SetPaLevel( NRF24L01_OutputPower_t OutPwr )
{
	// setar power amplifier
}

NRF24L01_OutputPower_t NRF24L01_GetPaLevel( void )
{
	// FAZER
	// get power amplifier
	
	return NRF24L01_OutputPower_0dBm;
}

void setDataRate( NRF24L01_DataRate_t DataRate )
{
	//FAZER
}

NRF24L01_DataRate_t NRF24L01_GetDataRate( void )
{
	// FAZER
	return NRF24L01_DataRate_1M;
}

void NRF24L01_SetRF( NRF24L01_DataRate_t DataRate, NRF24L01_OutputPower_t OutPwr) 
{
	uint8_t tmp = 0;
	NRF24L01_Struct.DataRate = DataRate;
	NRF24L01_Struct.OutPwr = OutPwr;
	
	if(DataRate == NRF24L01_DataRate_2M){
		tmp |= 1 << NRF24L01_RF_DR_HIGH;
	}else if(DataRate == NRF24L01_DataRate_250k){
		tmp |= 1 << NRF24L01_RF_DR_LOW;
	}
	/* If 1Mbps, all bits set to 0 */
	
	if(OutPwr == NRF24L01_OutputPower_0dBm){
		tmp |= 3 << NRF24L01_RF_PWR;
	}else if(OutPwr == NRF24L01_OutputPower_M6dBm){
		tmp |= 2 << NRF24L01_RF_PWR;
	}else if(OutPwr == NRF24L01_OutputPower_M12dBm){
		tmp |= 1 << NRF24L01_RF_PWR;
	}
	/* If -18dBm, all bits set to 0 */
	
	NRF24L01_WriteRegister(NRF24L01_REG_RF_SETUP, tmp);
}

void NRF24L01_SetAutoAck( uint8_t pipe, uint8_t state )																		
{
	if( pipe < 6 ){
		uint8_t temp = NRF24L01_ReadRegister( NRF24L01_REG_DEFAULT_VAL_EN_AA );
		
		if( state == 0x00 ){
			temp &= ~(1 << pipe);
		
		}else if( state == 0x01 ){
			temp |= (1 << pipe);
		}
		
		NRF24L01_WriteRegister( NRF24L01_REG_EN_AA, temp );
	
	// 0xFF disable/enable all pipes
	}else if( pipe == 0xFF ){
		uint8_t temp = NRF24L01_ReadRegister( NRF24L01_REG_DEFAULT_VAL_EN_AA );
		
		if( state == 0x00 ){
			temp &= ~(0x3F);
			
		}else if( state == 0x01 ){
			temp |= (0x3F);
		}
		
		NRF24L01_WriteRegister( NRF24L01_REG_EN_AA, temp );
	}
}

uint8_t NRF24L01_Available( uint8_t* pipe_num )					// REVER
{	
	uint8_t temp = NRF24L01_GetStatus();
	
	uint8_t result = ( temp & (1<<NRF24L01_RX_DR) );
	
	// return what pipe there is data to be read
	if( pipe_num ){
		*pipe_num = ( temp & (7<<NRF24L01_RX_P_NO) );
	}
	
	NRF24L01_WriteRegister( NRF24L01_REG_STATUS, (1<< NRF24L01_RX_DR) );
	
	// When there is payload on the acknowledge - Handle ack payload receipt
  if ( temp & (1<<NRF24L01_TX_DS) ){
		NRF24L01_WriteRegister( NRF24L01_REG_STATUS, (1<<NRF24L01_TX_DS) );
  }
	
	return result;
}

uint8_t NRF24L01_TestCarrier(void)
{	
	return (NRF24L01_ReadRegister(NRF24L01_REG_RPD) & 1);
}

void NRF24L01_OpenWritingPipe( uint8_t* address )
{
	NRF24L01_SetTxAddress( address );
	
	NRF24L01_WriteRegister( NRF24L01_REG_RX_PW_P0, NRF24L01_Struct.PayloadSize );	
}

void NRF24L01_StartListening(void)
{
	// Power up and set primary rx
	NRF24L01_WriteRegister( NRF24L01_REG_CONFIG, (NRF24L01_ReadRegister(NRF24L01_REG_CONFIG) | (1<<NRF24L01_PWR_UP) | (1<<NRF24L01_PRIM_RX)) );
	
	NRF24L01_ClearInterrupts();
	
	// Restore the pipe0 adddress, if exists
  //if (pipe0_reading_address)
    //write_register(RX_ADDR_P0, reinterpret_cast<const uint8_t*>(&pipe0_reading_address), 5);

	NRF24L01_FlushRx();
	NRF24L01_FlushTx();
	
	// Go to RX MODE
	NRF24L01_CE_HIGH();
	
	// only need 130us to settle
	HAL_Delay(1);
	
}

void NRF24L01_StopListening(void)
{
	// Go to stand
	NRF24L01_CE_LOW();
	
	NRF24L01_FlushRx();
	NRF24L01_FlushTx();
}

void NRF24L01_OpenReadingPipe( uint8_t number, uint8_t* address ) // REVER  //TESTAR
{
	/* "If this is pipe 0, cache the address.  This is needed because
  openWritingPipe() will overwrite the pipe 0 address, so
  startListening() will have to restore it." - Coliz, J. */
	
	if( number == 0 ){
		for( int ind = 0 ; ind < 5 ; ind++ )
			pipe0_reading_address[ind] = address[ind];
	}
	
	if( number < 6 ){
		
		// Pipes 0 and 1 need all 40bits
		if( number < 2 ){
			NRF24L01_WriteRegisterMulti( (NRF24L01_REG_RX_ADDR_P0+number), address, 5 );			// First address + offset of pipes
		
		// Pipes 2,3,4,5 need just the LSB byte
		}else{
			NRF24L01_WriteRegister( (NRF24L01_REG_RX_ADDR_P0+number), address[4] );						// The last byte is the LSByte
			
		}
	}
}

uint8_t NRF24L01_Read(void* data)																									
{
	uint8_t txBuffer[ NRF24L01_Struct.PayloadSize + 1 ];
	
	/* Set buffer with read command and some data to just shift */
	txBuffer[0] = NRF24L01_CMD_R_RX_PAYLOAD;																	
	for( uint8_t ind = 0 ; ind < NRF24L01_Struct.PayloadSize ; ind++ )				// APAGAR - ver se array nao iniciado da erro
		txBuffer[ind+1] = 0x00;
	
	uint8_t *rxBuffer;
	
	/* Pull down chip select */
	NRF24L01_CSN_LOW();
	
	/* Read payload */	
	HAL_SPI_TransmitReceive( NRF24L01_Struct.hspi, txBuffer, rxBuffer, (NRF24L01_Struct.PayloadSize+1), NRF24L01_SPI_TIMEOUT );				// (TROCAR) ordem caso de erro
	
	/* Pull up chip select */
	NRF24L01_CSN_HIGH();
	
	/* Reset status register, clear RX_DR interrupt flag */
	NRF24L01_WriteRegister(NRF24L01_REG_STATUS, (1 << NRF24L01_RX_DR));
	
	memcpy( data, &rxBuffer[1], NRF24L01_Struct.PayloadSize );
	
	return (NRF24L01_ReadRegister(NRF24L01_REG_FIFO_STATUS) & (1<<NRF24L01_RX_EMPTY));
}

uint8_t NRF24L01_GetPayloadSize(void)
{
	return NRF24L01_Struct.PayloadSize;
}

uint8_t NRF24L01_GetDynamicPayloadSize(void)
{
	
	return 0;
}

void NRF24L01_SetRetries(uint8_t delay, uint8_t count)
{
	
}
