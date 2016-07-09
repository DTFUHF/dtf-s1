
#include "board.h"

void CC2500_WriteReg(uint8_t address, uint8_t data)
{
  spi1Select();
  spi1_txrx_byte(address);
  spi1_txrx_byte(data);
  spi1Unselect();
}

uint8_t CC2500_ReadReg(uint8_t address)
{
  uint8_t buf = (CC2500_READ_SINGLE | address);
  uint8_t ret;
    
  spi1Select();
  spi1_txrx_byte(buf);
  ret = spi1_txrx_byte(0x00);
  spi1Unselect();
    
  return ret;
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
  spi1_txrx_byte(strobe);
  spi1Unselect();
}

void CC2500_WriteTXData(uint8_t *packet, uint8_t length)
{
  CC2500_Strobe(CC2500_STROBE_SIDLE);
  CC2500_Strobe(CC2500_STROBE_SFTX); // flush TX buffer

  uint8_t tx = CC2500_WRITE_BURST | CC2500_REG_TXFIFO;
    
  spi1Select();
  spi1_txrx_byte(tx);
  spi1_tx(packet, length);
  spi1Unselect();
  
  CC2500_SetGPIOTxRxMode(TX_EN);
  CC2500_Strobe(CC2500_STROBE_STX); // initiate TX
}

void CC2500_ReadRXData(uint8_t *packet, uint8_t length)
{
  uint8_t rx = CC2500_READ_BURST | CC2500_REG_RXFIFO;
  
  spi1Select();
  spi1_txrx_byte(rx);
  spi1_rx(packet, length);
  spi1Unselect();
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
