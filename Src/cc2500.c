
#include "board.h"

void CC2500_WriteReg(uint8_t address, uint8_t data)
{
  #if 1
  spi1Select();
  __disable_irq(); // TODO: remove these. very dangerous
  HAL_SPI_Transmit(&hspi1, &address, 1, 2);
  HAL_SPI_Transmit(&hspi1, &data, 1, 2);
  __enable_irq();
  spi1Unselect();
  
  #endif
  #if 0 // below code breaks everything. ROFL.
  
  uint8_t tx[2] = {address, data};
  spi1Select();
  __disable_irq();
  HAL_SPI_Transmit(&hspi1, tx, 2, 2);
  __enable_irq();
  spi1Unselect();
  
  #endif
  #if 0
  spi1Select();
  HAL_SPI_Transmit_IT(&hspi1, &address, 1);
  while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY){};
  HAL_SPI_Transmit_IT(&hspi1, &data, 1);
  while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY){};
  spi1Unselect();
  #endif
}

uint8_t CC2500_ReadReg(uint8_t address)
{
  uint8_t buf = (CC2500_READ_SINGLE | address);
  
  spi1Select();
  __disable_irq();
  HAL_SPI_Transmit(&hspi1, &buf, 1, 2);
  HAL_SPI_TransmitReceive(&hspi1, &buf, &buf, 1, 2);
  __enable_irq();
  spi1Unselect();
  return buf;
}

void CC2500_Reset(void)
{
  CC2500_Strobe(CC2500_STROBE_SRES);
  HAL_Delay(1);
  CC2500_SetGPIOTxRxMode(TXRX_OFF);
}

void CC2500_Strobe(uint8_t strobe)
{
  spi1Select();
  __disable_irq();
  HAL_SPI_Transmit(&hspi1, &strobe, 1, 2);
  __enable_irq();
  spi1Unselect();
}

void CC2500_WriteTXData(uint8_t *packet, uint8_t length)
{
  CC2500_Strobe(CC2500_STROBE_SIDLE);
  CC2500_Strobe(CC2500_STROBE_SFTX); // flush TX buffer

  uint8_t tx = CC2500_WRITE_BURST | CC2500_REG_TXFIFO;
  #if 1
  uint8_t zeroes[MAX_PACKET_SIZE] = {0}; // fix this obv

  spi1Select();
  __disable_irq();
  HAL_SPI_Transmit(&hspi1, &tx, 1, 2);
  HAL_SPI_TransmitReceive(&hspi1, packet, zeroes, length, 2);
  __enable_irq();
  spi1Unselect();
  #endif
  #if 0 // this doesnt work..... ???
  spi1Select();
  HAL_SPI_Transmit(&hspi1, &tx, 1, 2);
  HAL_SPI_Transmit_DMA(&hspi1, packet, length);
  while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY){};
  spi1Unselect();
  #endif
  
  CC2500_SetGPIOTxRxMode(TX_EN);
  CC2500_Strobe(CC2500_STROBE_STX); // initiate TX
}

void CC2500_ReadRXData(uint8_t *packet, uint8_t length)
{
  uint8_t rx = CC2500_READ_BURST | CC2500_REG_RXFIFO;

  #if 0
  uint8_t zeroes[MAX_PACKET_SIZE] = {0}; // fix this obv
  spi1Select();
  __disable_irq(); // this doesnt work because it also disables systick.
  HAL_SPI_Transmit(&hspi1, &rx, 1, 2);
  HAL_SPI_TransmitReceive(&hspi1, zeroes, packet, length, 2);
  __enable_irq();
  spi1Unselect();
  #endif
  #if 1
  spi1Select();
  HAL_SPI_Transmit(&hspi1, &rx, 1, 2);
  HAL_SPI_Receive_DMA(&hspi1, packet, length);
  while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY){};
  spi1Unselect();
  #endif
  #if 0 // this causes problems every 20 seconds.... what.
  spi1Select();
  HAL_SPI_Transmit(&hspi1, &rx, 1, 2);
  for (uint8_t i = 0; i < length; i++)
    {
      HAL_SPI_TransmitReceive(&hspi1, zeroes, &packet[i], 1, 2);
    }
  spi1Unselect();
  #endif
}

void CC2500_SetChan(uint8_t chan)
{
  CC2500_Strobe(CC2500_STROBE_SIDLE);
  CC2500_WriteReg(CC2500_REG_CHANNR, chan);
  CC2500_Strobe(CC2500_STROBE_SCAL);
  HAL_Delay(2);
}

void CC2500_SetGPIOTxRxMode(enum TXRX_State state)
{
  // cant this whole thing be done automatically with 0x1B and 0x1C
  if(state == TX_EN) {
    HAL_GPIO_WritePin(CC2500_RX_EN_GPIO, CC2500_RX_EN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CC2500_TX_EN_GPIO, CC2500_TX_EN_PIN, GPIO_PIN_SET);
  } else if (state == RX_EN) {
    HAL_GPIO_WritePin(CC2500_TX_EN_GPIO, CC2500_TX_EN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CC2500_RX_EN_GPIO, CC2500_RX_EN_PIN, GPIO_PIN_SET);
  } else { // rx is still on but LNA is off
    HAL_GPIO_WritePin(CC2500_TX_EN_GPIO, CC2500_TX_EN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CC2500_RX_EN_GPIO, CC2500_RX_EN_PIN, GPIO_PIN_RESET);
  }
}
