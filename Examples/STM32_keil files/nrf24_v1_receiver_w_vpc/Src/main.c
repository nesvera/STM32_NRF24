/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
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
#include "stm32f1xx_hal.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"


/* USER CODE BEGIN Includes */

#include "nrf24l01.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

struct structPackage{			// Size should not exceed 32Bytes
	uint8_t xAxis;
	uint8_t yAxis;
	uint8_t zAxis;
	uint8_t text[28];
} txPackage;

// Transmitter pipe address - Same of one of the pipe of receiver
uint8_t pipeOut[] = { 0xE7, 0xE6, 0xE5, 0xE4, 0xE3 };

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
	MX_USB_DEVICE_Init();

  /* USER CODE BEGIN 2 */

	// Need to declare pins and ports of nrf24l01
	NRF24L01_Pins_t NRF24L01_Pins;
	
	NRF24L01_Pins.CE.GPIOx = GPIOB;						
	NRF24L01_Pins.CE.GPIO_Pin = GPIO_PIN_0;
	NRF24L01_Pins.CSN.GPIOx = GPIOA;								// Declara pinos especificos da spi
	NRF24L01_Pins.CSN.GPIO_Pin = GPIO_PIN_4;
	NRF24L01_Pins.IRQ.GPIOx = NULL;									// Interrupt pin is not used yet
	NRF24L01_Pins.IRQ.GPIO_Pin = NULL;
	NRF24L01_Pins.MISO.GPIOx = GPIOA;
	NRF24L01_Pins.MISO.GPIO_Pin = GPIO_PIN_6;
	NRF24L01_Pins.MOSI.GPIOx = GPIOA;
	NRF24L01_Pins.MOSI.GPIO_Pin = GPIO_PIN_7;
	NRF24L01_Pins.SCK.GPIOx = GPIOA;
	NRF24L01_Pins.SCK.GPIO_Pin = GPIO_PIN_5;
	
	uint8_t channel = 80;											// Channel(decimal)
	uint8_t payload = sizeof(txPackage);
	
	/* Initialize NRF24L01 on channel 80 and n bytes of payload */
  /* By default 2Mbps data rate and 0dBm output power */
  NRF24L01_Init( channel, payload, &hspi1, NRF24L01_Pins );			//REVER

	/* Set 2MBps data rate and -18dBm output power */
  NRF24L01_SetRF( NRF24L01_DataRate_1M, NRF24L01_OutputPower_0dBm);

	/* Enable auto acknolodgment */
	NRF24L01_SetAutoAck(0xFF, 0);									// 0xFF Enable all or if you want enable just pipe 3, insert 3
																								// Have some troble with ack enable... many lost packages
		
	/* Enable CRC 2bytes */
	NRF24L01_SetCrcLength( NRF24L01_CRC_16 );

	//APAGAR
		NRF24L01_TestCarrier();

	/* Open a reading pipe with an address*/
	NRF24L01_OpenReadingPipe(1, pipeOut);
	HAL_Delay(1);
	
	NRF24L01_StartListening();
	HAL_Delay(1);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	uint32_t received_at = 0;					// hold the time of the last package received		
	
	txPackage.xAxis = 0;
	txPackage.yAxis = 0;
	txPackage.zAxis = 0;
	memset( &txPackage.text, '\0', sizeof(txPackage.text)); // clean variables
	
	uint8_t cont = 0;
	uint8_t strOut[256];
		

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		if( HAL_GetTick() - received_at > 2000 ){					// lost signal			
			if(cont<2){
				memset( strOut, '\0', sizeof(strOut) );
				//snprintf( (char *)strOut, sizeof(strOut), "Sem sinal! \n");
				//CDC_Transmit_FS( (uint8_t*)strOut, strlen( (char *)strOut) );
				
				cont++;
				HAL_Delay(1);
			}
		}
		
		if( NRF24L01_Available( NULL ) ){
			uint8_t done = 0;
			
			while( !done ){
				done = NRF24L01_Read(&txPackage);
				received_at = HAL_GetTick();		
			}
			
//			memset( strOut, '\0', sizeof(strOut) );
//			snprintf( (char *)strOut, sizeof(strOut), "x: %d \n", txPackage.xAxis );
//			CDC_Transmit_FS( (uint8_t*)strOut, strlen( (char *)strOut) );	
//			
//			memset( strOut, '\0', sizeof(strOut) );
//			snprintf( (char *)strOut, sizeof(strOut), "y: %d \n", txPackage.yAxis );
//			CDC_Transmit_FS( (uint8_t*)strOut, strlen( (char *)strOut) );
//			
//			memset( strOut, '\0', sizeof(strOut) );
//			snprintf( (char *)strOut, sizeof(strOut), "z: %d \n", txPackage.zAxis );
//			CDC_Transmit_FS( (uint8_t*)strOut, strlen( (char *)strOut) );
			
			memset( strOut, '\0', sizeof(strOut) );
			snprintf( (char *)strOut, sizeof(strOut), "x: %d, y: %d, z: %d \n", txPackage.xAxis, txPackage.yAxis, txPackage.zAxis );
			//CDC_Transmit_FS( (uint8_t*)strOut, strlen( (char *)strOut) );
			CDC_Transmit_FS( txPackage.text, strlen( (char *)txPackage.text ));
			
			// print received data
			
			cont = 0;
			
		}
		
		
		
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&hspi1);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
