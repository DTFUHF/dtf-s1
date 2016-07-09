
#include "board.h"

void spi1Select()
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
}

void spi1Unselect()
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}

void initSpi1(void)
{
  spi1Unselect();
  
  // init gpio clock and pins.
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  GPIOB->MODER &= ~(GPIO_MODER_MODER3 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5);
  GPIOB->MODER |= GPIO_MODER_MODER3_1 | GPIO_MODER_MODER4_1 | GPIO_MODER_MODER5_1; // set gpio AF state
  GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR3 | GPIO_OSPEEDR_OSPEEDR4 | GPIO_OSPEEDR_OSPEEDR5; // high output drive
  GPIOB->AFR[0] &= ~(GPIO_AFRL_AFRL3 | GPIO_AFRL_AFRL4 | GPIO_AFRL_AFRL5); // set AF0
  
  // init spi
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  SPI1->CR1 = SPI_CR1_SSM  | SPI_CR1_SSI | SPI_CR1_BR_1 | SPI_CR1_MSTR; // /8 div = 6MHz spi
  SPI1->CR2 = SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2 | SPI_CR2_FRXTH; // 8bit
  SPI1->CR1 |= SPI_CR1_SPE;
}

uint8_t spi1_txrx_byte(uint8_t byte)
{
  volatile uint8_t ret;
  
  ret = *(__IO uint8_t *)&SPI1->DR;
  
  *(__IO uint8_t *)&SPI1->DR = byte;
  
  while (!(SPI1->SR & SPI_SR_RXNE));
  
  ret = *(__IO uint8_t *)&SPI1->DR;
  
  while (SPI1->SR & SPI_SR_BSY);
  while (!(SPI1->SR & SPI_SR_TXE));
  return ret;
}

//void spi1_txrx_no_nss(uint8_t * tx, uint8_t * rx, uint8_t length)
//{
//  for (uint8_t i = 0; i < length; i++)
//  {
//    rx[i] = spi1_txrx_byte(tx[i]);
//  }
//}

void spi1_tx(uint8_t * tx, uint8_t length)
{
  for (uint8_t i = 0; i < length; i++)
  {
    spi1_txrx_byte(tx[i]);
  }
}

void spi1_rx(uint8_t * rx, uint8_t length)
{
  for (uint8_t i = 0; i < length; i++)
  {
    rx[i] =  spi1_txrx_byte(0x00);
  }
}

//void spi1_txrx(uint8_t * tx, uint8_t * rx, uint8_t length)
//{
//  spi1Select();
//  spi1_txrx_no_nss(tx, rx, length);
//  spi1Unselect();
//}
