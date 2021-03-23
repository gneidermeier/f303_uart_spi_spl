/**
 ******************************************************************************
 * @file    usart.c
 * @author  GN
 * @version V1
 * @date    23-March-2021
 * @brief   usart
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

/** @addtogroup usart
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*
 * https://community.st.com/s/question/0D50X00009XkiNwSAJ/stm32f3-discovery-recieve-char-or-word-from-rx
 */
/**************************************************************************************/
void RCC_Configuration(void) ;

void GPIO_Configuration(void) ;

void USARTx_Configuration(void) ;

char USART_GetChar(void) ;

void USART_PutChar(char i) ;

void USART_PutString(char *s) ;

void USART_PutDecimal(int i) ;

/**
 * @}
 */

/*****************************END OF FILE****/
