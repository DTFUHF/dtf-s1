
#include "board.h"

 #if defined ( __CC_ARM )
 __align(4) packet_union rxmsg;
 __align(4) packet_union txmsg;
 #else
 packet_union rxmsg __attribute__ ((aligned (4)));
 packet_union txmsg __attribute__ ((aligned (4)));
 #endif


void frsky_rx_select_ant(uint8_t ant)
{
  HAL_GPIO_WritePin(CC2500_ANT_SEL_GPIO, CC2500_ANT_SEL_PIN, (ant == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void frsky_rx_toggle_ant()
{
  HAL_GPIO_TogglePin(CC2500_ANT_SEL_GPIO, CC2500_ANT_SEL_PIN);
}

int16_t frsky_getrssidbm(uint8_t r)
{
  // TODO: change this to return percentages, which is actually useful to us. linearize dB and compare to a few points above noisefloor
  // rssi between -135 to -7dBm or so
  // calculate signed rssi, then take positive value to use as uint8_t
  int16_t rssi = r;
  // from cc2500 datasheet, copypasta
  if (rssi >= 128)
    rssi = ((rssi-256) / 2) - 70;
  else
    rssi = (rssi / 2) - 70;
  
  return (int16_t)(rssi); // -7 = strongest signal, -135 = weakest signal
}

void frsky_d8_rx_startup(void)
{
  frsky_d8_rx_init();
  CC2500_WriteReg(CC2500_REG_IOCFG0, 0x01);  // GD0 asserts when rx_fifo is above th. or packet is complete. maybe this should be in tx too?
  frsky_rx_select_ant(0);
}
