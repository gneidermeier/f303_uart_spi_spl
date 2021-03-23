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
#include "spi.h"
#include "usart.h"

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
void delay_ms(uint32_t nTime) {
    TimingDelay = nTime;
    while (TimingDelay != 0)
        ;
}

void _SysTick_Handler(void) {
    if (TimingDelay != 0x00) {
        TimingDelay--;
    }
}



/*
 * https://olayiwolaayinde.medium.com/getting-started-with-stm32-23139d3401b8
 */
void GPIO_Setup_LED(void) {
    GPIO_InitTypeDef GPIOx;

// Enables the peripheral clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

//configures the operating mode for the selected pins as Push-Pull Output
    GPIOx.GPIO_Mode = GPIO_Mode_OUT;
// GPIOx.GPIO_OType = GPIO_OType_PP; // default?
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
#endif
#if 1 // SPI
    PeripheralInit_SPIx_Slave();
#endif

    RCC_Configuration();
    GPIO_Configuration();
    USARTx_Configuration();

    /* Infinite loop */
    while (1) {

        n = (n < 0x40) ? n + 1 : 0x30;

#ifdef LEDTEST
    flag ^= 1;
    if (0 == flag) {
        GPIO_SetBits(GPIOA, GPIO_Pin_5); //Off the LED since the negative pin is connected to PB12
        // You can also use GPIO_WriteBit(GPIOB, GPIO_Pin_12, SET);
        } else {
        GPIO_ResetBits(GPIOA, GPIO_Pin_5); //On the LED since the negative pin is connected to PB12
        // You can also use GPIO_WriteBit(GPIOB, GPIO_Pin_12, RESET);
    }
#endif
        sprintf((char*) spi_out, "ABCDE"); // junk data
        sprintf((char*) spi_buf, "....."); // junk data
#if 1 // SPI
        spi_buf[0] = transfer_8b_SPIx_Slave(spi_out[0]);
        spi_buf[1] = transfer_8b_SPIx_Slave(spi_out[1]);
        spi_buf[2] = transfer_8b_SPIx_Slave(spi_out[2]);
        spi_buf[3] = transfer_8b_SPIx_Slave(n);
#endif
        sprintf(uart_out, ">%d: %02X %02X %02X %02X.\r\n", n, spi_buf[0],
                spi_buf[1], spi_buf[2], spi_buf[3]);
        //      uart_out[0] = n;

//  USART1->TDR = 0x30;
        USART_PutString(uart_out);

        delay_ms(100);
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
