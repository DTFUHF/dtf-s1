
#include "board.h"

extern packet_union rxmsg;

/* init stuff for bindmode. */
void frsky_rx_bind_init(void)
{
  // TODO: lower power on bind and disable LNA, then enable again in normal rx mode //  CC2500_WriteReg(CC2500_REG_PATABLE, 0x50);
  CC2500_WriteReg(CC2500_REG_ADDR, 0x03);
  CC2500_SetGPIOTxRxMode(RX_EN);
  CC2500_SetChan(FRSKY_BIND_CHANNEL);
  CC2500_Strobe(CC2500_STROBE_SRX);
}

uint8_t frsky_rx_binding(void)
{
  static int8_t binding_state = 0;
  static uint8_t binding_loops = 0;
  static uint8_t hopChannelsTemp[48]; // sized for d8 - why cant this be 47?
  
  if (HAL_GPIO_ReadPin(CC2500_GD0_GPIO, CC2500_GD0_PIN) == GPIO_PIN_SET)
  {
    CC2500_Strobe(CC2500_STROBE_SIDLE);
    
    uint8_t rx_packet_length = CC2500_ReadReg(CC2500_REG_RXBYTES | CC2500_READ_BURST);
    
    if (rx_packet_length && (rx_packet_length < MAX_PACKET_SIZE))
    {
      CC2500_ReadRXData(rxmsg.packet, rx_packet_length);
      
      if (binding_state == 0)
      {
        if ((rxmsg.bind_packet_data.header1 == 0x0103) && (rxmsg.packet[5] == 0x00) && (rxmsg.packet[19] & 0xA0)) // 0xA0 seems to be some mask, in lieu of crc decoding
        {
          m_config.frsky2way_bind_info.bind_id = rxmsg.bind_packet_data.header2;
          for (uint8_t i=0; i<5; i++)
          {
            hopChannelsTemp[i] = rxmsg.packet[6+i];
          }
          binding_state = 5;
        }
      } else {
        if ((rxmsg.bind_packet_data.header2 == m_config.frsky2way_bind_info.bind_id) && (rxmsg.packet[5] == binding_state) && (rxmsg.packet[19] & 0xA0)) // binding address check
        {
          for (uint8_t i=0; i<5; i++)
          {
            hopChannelsTemp[binding_state+i] = rxmsg.packet[6+i];
            
            if ((binding_state + i) == (NUM_HOP_CHANNELS - 1)) // end condition for d8
            {
              binding_state = -1;
              binding_loops++;
              if (binding_loops == 1)
              {
                for(uint8_t i=0; i<NUM_HOP_CHANNELS; i++) // copy temp array to working array for future compare
                {
                  m_config.frsky2way_bind_info.hopChannels[i] = hopChannelsTemp[i];
                }
              }
              break;
            }
          }
          if (binding_state > 0) { // to allow break state to leave this condition
            binding_state += 5;
          }
        }
      }
    }
    CC2500_Strobe(CC2500_STROBE_SFRX);
    CC2500_Strobe(CC2500_STROBE_SRX);
  }
  
  // in order to get a good bind, this routine should read the bindcodes several times to make sure it has teh correct ones. either that or reverse engineer the crc
  if ((binding_loops == 1) & (binding_state == -1))
  {
    binding_state = 0;
  }
  else if ((binding_loops > 1) & (binding_state == -1))
  {
    binding_state = 0;
    uint8_t data_good = 1;
    for(uint8_t i=0; i<NUM_HOP_CHANNELS; i++)
    {
      if(hopChannelsTemp[i] != m_config.frsky2way_bind_info.hopChannels[i])
      {
        binding_loops = 0;
        data_good = 0; // something is broken, stop immediately and go back.
        break;
      }
    }
    // bind data has been verified.
    if (data_good == 1)
    {
      binding_loops ++;
    }
    if (binding_loops == 3)
    {
      /* store eeprom - bind id and hopchannels, already copied into m_config */
      /* TODO: autodetect bind type will change this. */
      m_config.frsky2way_bind_info.bind_type = BIND_TYPE_D8;
      
      /* If failsafe type is autodetect, erase the failsafe values to allow stop-ppm mode. */
      if (m_config.failsafeType == FAILSAFE_AUTODETECT)
      {
        for (uint8_t i = 0; i < MAX_CHANNEL_OUTPUTS; i++)
        {
          m_config.failsafeValues[i] = 0;
        }
      }
      
      eeprom_write();
      HAL_GPIO_WritePin(LED_RED_GPIO, LED_RED_PIN, LED_RED_STATE_ON);
      HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, LED_GREEN_STATE_ON);
      return 1;
    }
  }
  return 0;
}
