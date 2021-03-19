/**
 ******************************************************************************
 * @file    main.c
 * @author  MCD Application Team
 * @version V1.2.2
 * @date    14-August-2015
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "stm32f30x_spi.h"

/** @addtogroup STM32F30x_StdPeriph_Templates
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*
 * https://learnbuildshare.wordpress.com/about/stm32/using-spi-as-slave/
 */
//static
__IO uint32_t TimingDelay; // __IO so no undesired optimization

// Delay for ms
void delay_ms(uint32_t nTime){
    TimingDelay = nTime;
    while(TimingDelay != 0);
}

void _SysTick_Handler(void) {
    if(TimingDelay != 0x00)
    {
    	TimingDelay--;
    }
}

// setup SPI1 pins A4/SS, A5/SCK, A6/MISO, A7/MOSI on port A
void PeripheralInit_SPI1_Slave() {

	GPIO_InitTypeDef GPIO_InitDef;
	SPI_InitTypeDef SPI_InitDef;

	// initialize init structs
	GPIO_StructInit(&GPIO_InitDef);
	SPI_StructInit(&SPI_InitDef);

	// initialize clocks
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1,
//			 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA,
			ENABLE);

	// Enables the peripheral clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    // SS
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_5);
    // SCK
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_5);
    // MISO
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_5);
    // MOSI
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_5);


	// initialize A4/SS alternate function open-drain (50 MHz)
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF; //  GPIO_Mode_AF_OD;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitDef);

	// initialize A5/SCK alternate function open-drain (50 MHz)
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF; // GPIO_Mode_AF_OD;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitDef);

	// initialize A6/MISO alternate function push-pull (50 MHz)
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF; //  GPIO_Mode_AF_PP;
	GPIO_InitDef.GPIO_OType = GPIO_OType_OD; // GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitDef);

	// initialize A7/MOSI alternate function open-drain (50 MHz)
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF; // GPIO_Mode_AF_OD;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitDef);

	//  initialize SPI slave
	// for slave, no need to define SPI_BaudRatePrescaler
	SPI_InitDef.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitDef.SPI_Mode = SPI_Mode_Slave;
	SPI_InitDef.SPI_DataSize = SPI_DataSize_8b; // 8-bit transactions
	SPI_InitDef.SPI_FirstBit = SPI_FirstBit_MSB; // MSB first
	SPI_InitDef.SPI_CPOL = SPI_CPOL_Low; // CPOL = 0, clock idle low
	SPI_InitDef.SPI_CPHA = SPI_CPHA_1Edge; //  SPI_CPHA_2Edge; // CPHA = 1
	SPI_InitDef.SPI_NSS = SPI_NSS_Hard; // use hardware SS
	SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; // APB2 72/64 = 1.125 MHz
	// SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // APB2 72/256 = 0.28 MHz
	// SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; // APB2 72/16 = 4.5 MHz
	SPI_InitDef.SPI_CRCPolynomial = 7;

//	SPI_I2S_DeInit(SPI1); // https://hackaday.io/project/28496-the-ultimate-vlogging-mic/log/71799-spi-between-the-raspberry-pi-stm32
	SPI_Init(SPI1, &SPI_InitDef);

	SPI_Cmd(SPI1, ENABLE);
}

// transfer a byte over SPI1  A4/SS, A5/SCK, A6/MISO, A7/MOSI
uint8_t transfer_8b_SPI1_Slave(uint8_t outByte) {

	// Approach 1, from Brown's book
	// SPI_I2S_SendData(SPI1, outByte); // send
	// while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	// return SPI_I2S_ReceiveData(SPI1); // read received

	// Approach 2,
	// from http://www.lxtronic.com/index.php/basic-spi-simple-read-write
	while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))
		;
//	SPI_I2S_SendData(SPI1, outByte);                                     // send
	SPI_SendData8(SPI1, outByte);

	while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE))
		;
//	return SPI_I2S_ReceiveData(SPI1);                          // read recieived
	return SPI_ReceiveData8(SPI1);
}

/*
 * https://community.st.com/s/question/0D50X00009XkiNwSAJ/stm32f3-discovery-recieve-char-or-word-from-rx
 */
/**************************************************************************************/
void RCC_Configuration(void) {

//	    RCC_USARTCLKConfig(RCC_USART1CLK_PCLK);

//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 /* | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA */, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//	 /* GPIOA clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/*
	 * https://github.com/travisg/stm32/blob/master/main.c

	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	 GPIO_PinRemapConfig(GPIO_Remap_USART1, DISABLE);
	 */
}

/**************************************************************************************/
void GPIO_Configuration(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	/*-------------------------- GPIO Configuration ----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Connect USART pins to AF */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7); // USART1_TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7); // USART1_RX
}

/**************************************************************************************/
void USARTx_Configuration(void) {
	USART_InitTypeDef USART_InitStructure;
	/* USARTx configuration ------------------------------------------------------*/
	/* USARTx configured as follow:
	 - BaudRate = 115200 baud
	 - Word Length = 8 Bits
	 - One Stop Bit
	 - No parity
	 - Hardware flow control disabled (RTS and CTS signals)
	 - Receive and transmit enabled
	 */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}

/**************************************************************************************/
char USART_GetChar(void) {
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		; // Wait for Full
	return ((char) USART_ReceiveData(USART1));
}
/**************************************************************************************/
void USART_PutChar(char i) {
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		; // Wait for Empty
	USART_SendData(USART1, i);
}
/**************************************************************************************/
void USART_PutString(char *s) {
	while (*s)
		USART_PutChar(*s++);
}
/**************************************************************************************/
void USART_PutDecimal(int i) {
	char str[16], *s;
	int n;
	s = str + sizeof(str); // Point to tail
	*--s = 0; // NUL
	if (i < 0) // Negative
			{
		n = 1;
		i = -i; // Negate
	} else
		n = 0;
	do {
		*--s = '0' + (char) (i % 10);
		i = i / 10;
	} while (i);
	if (n) // Add sign if negated
		*--s = '-';
	USART_PutString(s);
}
/**************************************************************************************/

/*
 * https://olayiwolaayinde.medium.com/getting-started-with-stm32-23139d3401b8
 */
void GPIO_Setup_LED(void)
{
	GPIO_InitTypeDef GPIOx;

// Enables the peripheral clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

//configures the operating mode for the selected pins as Push-Pull Output
	GPIOx.GPIO_Mode = GPIO_Mode_OUT; //  GPIO_Mode_Out_PP;

// GPIOx.GPIO_OType = GPIO_OType_PP;

	GPIOx.GPIO_Speed = GPIO_Speed_2MHz;
	GPIOx.GPIO_Pin = GPIO_Pin_5;

	GPIO_Init(GPIOA, &GPIOx);
}

//#define LEDTEST

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void) {
	/*!< At this stage the microcontroller clock setting is already configured,
	 this is done through SystemInit() function which is called from startup
	 file (startup_stm32f30x.s) before to branch to application main.
	 To reconfigure the default setting of SystemInit() function, refer to
	 system_stm32f30x.c file
	 */

	char uart_out[256] = "0123\r\n";
	uint8_t spi_out[16];
	uint8_t spi_buf[16];
	int n = 0x30;

	SysTick_Config(SystemCoreClock / 1000); // configure system clock for 1000 ticks/s or 1 ms

#ifdef LEDTEST
	int flag = 0;
	GPIO_Setup_LED();
#else
	PeripheralInit_SPI1_Slave();
#endif

	RCC_Configuration();
	GPIO_Configuration();
	USARTx_Configuration();

	/* Infinite loop */
	while (1) {

		n = (n < 0x3a) ? n + 1 : 0x30;

#ifdef LEDTEST
		flag ^= 1;
		if (0 == flag) {
			GPIO_SetBits(GPIOA, GPIO_Pin_5); //Off the LED since the negative pin is connected to PB12
			// You can also use GPIO_WriteBit(GPIOB, GPIO_Pin_12, SET);
		} else {
			GPIO_ResetBits(GPIOA, GPIO_Pin_5); //On the LED since the negative pin is connected to PB12
			// You can also use GPIO_WriteBit(GPIOB, GPIO_Pin_12, RESET);
		}
#else
		sprintf((char*) spi_out, "ABCDE"); // junk data
		sprintf((char*) spi_buf, "....."); // junk data
		spi_buf[0] = transfer_8b_SPI1_Slave(spi_out[0]);
		spi_buf[1] = transfer_8b_SPI1_Slave(spi_out[1]);
		spi_buf[2] = transfer_8b_SPI1_Slave(spi_out[2]);
		spi_buf[3] = transfer_8b_SPI1_Slave(spi_out[3]);

		sprintf(uart_out, ">%d: %02X %02X %02X %02X.\r\n", n, spi_buf[0],
				spi_buf[1], spi_buf[2], spi_buf[3]);
		//      uart_out[0] = n;
#endif

//	  USART1->TDR = 0x30;
		USART_PutString(uart_out);

		delay_ms(200);
	}
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
