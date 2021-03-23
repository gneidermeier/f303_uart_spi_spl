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
#include "stm32f30x_rcc.h"


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
void RCC_Configuration(void) {

#if 0 // UART1
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 /* | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA */, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    /* GPIOA clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

#else

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 ,ENABLE); // RCC_AHBPeriph_GPIOA

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE); // enables clock to portA (https://www.mikrocontroller.net/attachment/183014/main.c)

#endif
}

/**************************************************************************************/
void GPIO_Configuration(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    /*-------------------------- GPIO Configuration ----------------------------*/
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; // USART1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; // USART2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Connect USART pins to AF */
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7); // USART1_TX
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7); // USART1_RX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7); // USART2_TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_7); // USART2_RX
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
// enable USART 1
//    USART_Init(USART1, &USART_InitStructure);
//    USART_Cmd(USART1, ENABLE);
// enable USART 2
    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);
}

/**************************************************************************************/
char USART_GetChar(void) {
//    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
//        ; // Wait for Full
//    return ((char) USART_ReceiveData(USART1));

    while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
        ; // Wait for Full
    return ((char) USART_ReceiveData(USART2));
}
/**************************************************************************************/
void USART_PutChar(char i) {
//    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
//        ; // Wait for Empty
//    USART_SendData(USART1, i);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
        ; // Wait for Empty
    USART_SendData(USART2, i);
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
/**
 * @}
 */
/*****************************END OF FILE****/
