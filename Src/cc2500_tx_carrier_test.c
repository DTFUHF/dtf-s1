
#include "board.h"

void tx_carrier_test(void)
{
  frsky2way_init(); // 2 403 999 847Hz
  CC2500_SetChan(0);
  CC2500_WriteReg(CC2500_REG_IOCFG0, 0x2D); // gd0 as input
  CC2500_WriteReg(CC2500_REG_PKTCTRL0, 0x32); // data in on GD0
  
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = CC2500_GD0_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CC2500_GD0_GPIO, &GPIO_InitStruct);
  HAL_GPIO_WritePin(CC2500_GD0_GPIO, CC2500_GD0_PIN, GPIO_PIN_SET);
  
  CC2500_SetGPIOTxRxMode(TX_EN);
  CC2500_Strobe(CC2500_STROBE_STX);
  
  while(1){};
}
