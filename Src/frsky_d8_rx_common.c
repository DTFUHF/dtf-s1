
#include "board.h"

uint8_t fs1cal[D8_NUM_HOP_CHANNELS], fs2cal[D8_NUM_HOP_CHANNELS], fs3cal[D8_NUM_HOP_CHANNELS];

/********************************
* reset cc2500 and init registers
*********************************/

void frsky_d8_rx_init(void)
{
  CC2500_Reset();
  
  uint8_t id = CC2500_ReadReg(CC2500_REG_PARTNUM);
  
  if (id != 0x80) // TODO: remove this, replace with something better
    while(1);

  for (uint8_t i = 0; i < D8_RX_CC2500_INIT_ARRAY_SIZE; i++)
    CC2500_WriteReg(d8_rx_initArray[i][0], d8_rx_initArray[i][1]);

  CC2500_Strobe(CC2500_STROBE_SIDLE);
  CC2500_Strobe(CC2500_STROBE_SFRX);
  CC2500_Strobe(CC2500_STROBE_SFTX);
}

void frsky_full_cal(void) // TODO: remove fs1cal and fs2cal tables as theres only one value for all channels
{
  CC2500_Strobe(CC2500_STROBE_SIDLE);
  HAL_Delay(2);
  for (uint8_t i = 0; i < D8_NUM_HOP_CHANNELS; i++)
  {
    CC2500_WriteReg(CC2500_REG_CHANNR, m_config.frsky_d8_bind_info.hopChannels[i]);
    CC2500_Strobe(CC2500_STROBE_SCAL);
    HAL_Delay(2);
    fs1cal[i] = CC2500_ReadReg(CC2500_REG_FSCAL1);
    fs2cal[i] = CC2500_ReadReg(CC2500_REG_FSCAL2);
    fs3cal[i] = CC2500_ReadReg(CC2500_REG_FSCAL3) & 0xCF; // disable charge pump calibration
  }
}

void frsky_set_hopchan(uint8_t hopchan)
{
  CC2500_Strobe(CC2500_STROBE_SIDLE);
  CC2500_WriteReg(CC2500_REG_FSCAL1, fs1cal[hopchan]);
  CC2500_WriteReg(CC2500_REG_FSCAL2, fs2cal[hopchan]);
  CC2500_WriteReg(CC2500_REG_FSCAL3, fs3cal[hopchan]);
  CC2500_WriteReg(CC2500_REG_CHANNR, m_config.frsky_d8_bind_info.hopChannels[hopchan]);
}

void frsky_rx_enable(uint8_t hopchan)
{
  CC2500_SetGPIOTxRxMode(RX_EN);
  frsky_set_hopchan(hopchan);
  CC2500_Strobe(CC2500_STROBE_SRX);
}
