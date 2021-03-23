/**
 ******************************************************************************
 * @file    spi.c
 * @author  GN
 * @version V1
 * @date    23-March-2021
 * @brief   spi
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_spi.h"

/** @addtogroup spi
 * @{
 */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

// setup SPI1 pins A4/SS, A5/SCK, A6/MISO, A7/MOSI on port A
// see also  https://hackaday.io/project/28496-the-ultimate-vlogging-mic/log/71799-spi-between-the-raspberry-pi-stm32
//    https://community.st.com/s/question/0D50X00009Xkgnm/stm32f0-spi
//
void PeripheralInit_SPIx_Slave() {

    GPIO_InitTypeDef GPIO_InitDef;
    SPI_InitTypeDef SPI_InitDef;

    // initialize init structs
    GPIO_StructInit(&GPIO_InitDef);
    SPI_StructInit(&SPI_InitDef);

    // initialize clocks
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE);
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
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitDef);

    // initialize A5/SCK alternate function open-drain (50 MHz)
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitDef);

    // initialize A6/MISO alternate function push-pull (50 MHz)
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitDef.GPIO_OType = GPIO_OType_OD;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitDef);

    // initialize A7/MOSI alternate function open-drain (50 MHz)
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;
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

// transfer a byte over SPI
uint8_t transfer_8b_SPIx_Slave(uint8_t outByte) {

    // Approach 1, from Brown's book
    // SPI_I2S_SendData(SPI1, outByte); // send
    // while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    // return SPI_I2S_ReceiveData(SPI1); // read received

    // Approach 2,
    // from http://www.lxtronic.com/index.php/basic-spi-simple-read-write
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))
        ;
// SPI_I2S_SendData(SPI1, outByte);                                     // send
    SPI_SendData8(SPI1, outByte);

    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE))
        ;
// return SPI_I2S_ReceiveData(SPI2);                          // read recieived
    return SPI_ReceiveData8(SPI1);
}
/**
 * @}
 */
/*****************************END OF FILE****/
