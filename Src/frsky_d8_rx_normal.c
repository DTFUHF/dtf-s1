
#include "board.h"

extern packet_union rxmsg;
extern packet_union txmsg;
extern output_handle_functions_t output_handle_functions;

uint8_t rssi;
uint8_t num_frsky2way_channels; // 1-indexed
//uint8_t rssidbm;
static int8_t rssichan = -1; // zero indexed, -1 = not used
volatile uint16_t channelValues[MAX_CHANNEL_OUTPUTS] = {0}; // TODO: make this 1500? or have sbus set its own default values?
uint8_t failsafeStatus = 0; // TODO: replace with enum. needs to be here so other functions can check failsafe state

enum {
  STATE_WILL_RX = 0,
  STATE_RXING,
  STATE_RXD,
  STATE_PREPARE_TX,
  STATE_WILL_TX,
  STATE_TXING,
  STATE_TXD,
  NUM_RX_STATES,
};

// note: this state indicates the next action to take. the last pkt received will be 1 less
enum {
  FRSKY_RX1 = 0,
  FRSKY_RX2,
  FRSKY_RX3,
  FRSKY_TX_TELEM,
  NUM_FRSKY_STATES,
};

enum {
  STATE_SEARCH = 0,
  STATE_RUNNING,
};

/* init stuff for normal RXing. */
void frsky_d8_rx_normal_init(void)
{
  CC2500_WriteReg(CC2500_REG_ADDR, (uint8_t)m_config.frsky_d8_bind_info.bind_id);
  frsky_full_cal();
  
  switch(m_config.rssi_injection_type)
  {
    default:
      num_frsky2way_channels = 8;
      break;
    case RSSI_INJECTION_AUTO:
      switch(m_config.frsky_d8_bind_info.bind_type)
      {
        default:
        case BIND_TYPE_D8:
          rssichan = 8; // inject on channel 9 by default.
          num_frsky2way_channels = 9;
          break;
      }
      break;
    case RSSI_INJECTION_NONE:
      switch(m_config.frsky_d8_bind_info.bind_type)
      {
        default:
        case BIND_TYPE_D8:
          num_frsky2way_channels = 8;
          break;
      }
      break;
    case RSSI_INJECTION_CHAN:
      rssichan = m_config.rssi_injection_chan;
      switch(m_config.frsky_d8_bind_info.bind_type)
      {
        default:
        case BIND_TYPE_D8:
          if (rssichan > 7)
            num_frsky2way_channels = rssichan + 1;
          break;
      }
      break;
  }
}

uint8_t frsky_d8_rx_getpkt(void)
{
  uint8_t ret = 0;
  if (HAL_GPIO_ReadPin(CC2500_GD0_GPIO, CC2500_GD0_PIN) == GPIO_PIN_SET)
  {
    // note: can't read this while the packet is still being received.
    // HAL_Delay(1); // but this is just silly.
    CC2500_Strobe(CC2500_STROBE_SIDLE);
    
    volatile uint8_t rx_packet_length = CC2500_ReadReg(CC2500_REG_RXBYTES | CC2500_READ_BURST);
    
    if ((rx_packet_length > 7) && (rx_packet_length < D8_MAX_PACKET_SIZE)) // TODO: fix this to detect pktlen from bindmode
    {
      CC2500_ReadRXData(rxmsg.packet, rx_packet_length);
      
      if ((rxmsg.d8_rx_packet_data.header_bindcode == m_config.frsky_d8_bind_info.bind_id) && (rxmsg.d8_rx_packet_data.len == 0x11))
        /* So, there's two bytes in the packet header that indicate something.
         * 0x0100 - North American launch day taranis A
         *        - DJT jr module
         * 0x0300 - XJT jr module
         * There are others out there which are unknown, so this check is removed for now.
         * Also, there's a CRC or something at the end. But it isn't. 0xA0 bits will always match it. */
        ret = rx_packet_length;
    }
    CC2500_Strobe(CC2500_STROBE_SFRX);
  }
  
  return ret;
}

void frsky2way_d8_build_telem_packet(void)
{
  for ( uint8_t i = 0; i < D8_MAX_PACKET_SIZE; i++ )
  {
    txmsg.packet[i] = 0;
  }
  //  byte 0   = packet byte length not including 2byte crc
  txmsg.packet[0] = 17;
  //  byte 1,2 = bind code
  txmsg.d8_telem_packet_data.header_bindcode = m_config.frsky_d8_bind_info.bind_id;
  //  byte 3   = A1 52mV per unit
  txmsg.packet[3] = 0x00; // dummy
  //  byte 4   = A2 13.4mV per unit
  txmsg.packet[4] = 0x00; // dummy
  //  byte 5   = rssi
  //txmsg.packet[5] = 0x60; // dummy data, i think this sends back raw rssi values (but they are a bit too low). opentx seems to want value 0-100 (%?) with alarm at 45 TODO:
  // values 128-255 cause constant beep; 0-44 pulsing beep; 45-127 no beep (on XJT)
  // 128-255 do not show as negative on taranis rssi telemetry page, maybe only 7 bits needed.
  txmsg.packet[5] = 0x7f & rssi; // TODO: fix
  // other bytes as shown in frsky sport pdf
}

void frsky_rx_process_channels(void)
{
  // parse RC data here. packet[6] thru packet[17]
  // assuming frsky D8 at the moment
  
  // sadly c does not support arrays of bitfields
  channelValues[0] = (uint16_t)(((uint16_t)rxmsg.d8_rx_packet_data.ch1_low | ((uint16_t)rxmsg.d8_rx_packet_data.ch1_high << 8)) * 0.6666);
  channelValues[1] = (uint16_t)(((uint16_t)rxmsg.d8_rx_packet_data.ch2_low | ((uint16_t)rxmsg.d8_rx_packet_data.ch2_high << 8)) * 0.6666);
  channelValues[2] = (uint16_t)(((uint16_t)rxmsg.d8_rx_packet_data.ch3_low | ((uint16_t)rxmsg.d8_rx_packet_data.ch3_high << 8)) * 0.6666);
  channelValues[3] = (uint16_t)(((uint16_t)rxmsg.d8_rx_packet_data.ch4_low | ((uint16_t)rxmsg.d8_rx_packet_data.ch4_high << 8)) * 0.6666);
  channelValues[4] = (uint16_t)(((uint16_t)rxmsg.d8_rx_packet_data.ch5_low | ((uint16_t)rxmsg.d8_rx_packet_data.ch5_high << 8)) * 0.6666);
  channelValues[5] = (uint16_t)(((uint16_t)rxmsg.d8_rx_packet_data.ch6_low | ((uint16_t)rxmsg.d8_rx_packet_data.ch6_high << 8)) * 0.6666);
  channelValues[6] = (uint16_t)(((uint16_t)rxmsg.d8_rx_packet_data.ch7_low | ((uint16_t)rxmsg.d8_rx_packet_data.ch7_high << 8)) * 0.6666);
  channelValues[7] = (uint16_t)(((uint16_t)rxmsg.d8_rx_packet_data.ch8_low | ((uint16_t)rxmsg.d8_rx_packet_data.ch8_high << 8)) * 0.6666);
  
  output_handle_functions.outputUpdate();
}

void frsky_d8_rx_loop(void)
{
  // TODO: disable LNA on high power rxing
  // TODO: rssi injection (and settings)
  
  //36ms loop. rx 3 times every 9ms. then ~2ms, then tx a packet (~6ms?). then wait for next packet, continue cycle.
  static uint8_t frsky_state = FRSKY_RX1; // can be derived from counter
  static uint8_t hopchan = 0; // can be derived from counter
  static uint8_t rx_state = STATE_WILL_RX;
  static uint8_t fail_state = STATE_SEARCH;
  static uint32_t lastPktTime = 0; // time last packet was received, used mainly for failasfe.
  static uint32_t nextPktTime = 200; // time of next expected incoming packet. default: approx. 200ms startup time, to not fuck shit up, this is still hacky and shitty tho
  static uint32_t lastHopTime = 0; // time of last hop, normally every 9ms, in searchmode every 45ms. telem txing places this in the middle of its packet frame for simplicity
  static uint32_t txStartTime = 0; // time when the tx packet should start, usually 2ms after an RX3 was received.
  static uint8_t gotFirstPkt = 0;
  static uint32_t failsafeBtnTime = 0;
  static uint8_t failsafeBtnSettingStatus = 0;
  
  uint32_t time = HAL_GetTick();
  uint8_t freqoff_est;
  
  if (m_config.frsky_d8_bind_info.bind_type == BIND_TYPE_NONE)
  {
    /* do nothing if not bound. */
    return;
  }
  
  switch (rx_state)
  {
    case STATE_WILL_RX:
      frsky_rx_enable(hopchan);
      rx_state = STATE_RXING;
      break;
      /* end STATE_WILL_RX */
    
    case STATE_RXING:
      if (frsky_d8_rx_getpkt() != 0)
      {
        rx_state = STATE_RXD;
        break;
      }
      
      switch (fail_state)
      {
        case STATE_RUNNING:
          /* If we expected a packet and didn't get it, take action after a timeout. */
          if (time > (nextPktTime + 2))
          {
            /* Here we 'emulate' a packet RXing in order to prepare for the next action. */
            /* lastHopTime is updated to the time the packet should have been received, which is the current nextPktTime. */
            /* nextPktTime is updated to reflect the next anticipated packet, in 9 or 18ms. */
            /* lastPktTime is untouched, as no real RXing was done. */
            hopchan = (hopchan + 1) % D8_NUM_HOP_CHANNELS;
            frsky_state = (frsky_state + 1) % NUM_FRSKY_STATES;
            frsky_rx_toggle_ant();
            lastHopTime = nextPktTime;
            if (frsky_state == FRSKY_TX_TELEM)
            {
              nextPktTime = nextPktTime + 18;
              rx_state = STATE_PREPARE_TX;
            } else {
              nextPktTime = nextPktTime + 9;
              rx_state = STATE_WILL_RX;
            }
          }
          /* if it has been a long time since the last valid packet, go to a different hop sequence in an attempt to find the tx's hop phase. */
          /* TODO: make this timeout configurable, maybe */
          if (time > (lastPktTime + 1000)) 
            fail_state = STATE_SEARCH;
          break;
          /* end STATE_RUNNING */
          
        case STATE_SEARCH:
          HAL_GPIO_WritePin(LED_RED_GPIO, LED_RED_PIN, LED_RED_STATE_ON);
          if (gotFirstPkt > 1)
            HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, LED_GREEN_STATE_OFF);
          /* hop every 45ms while looking for packets */
          if (time > (lastHopTime + 45))
          {
            hopchan = (hopchan + 1) % D8_NUM_HOP_CHANNELS;
            
            /* TODO: allow TXing while in failsafe. maybe? */
            frsky_state = FRSKY_RX1;
            
            lastHopTime = HAL_GetTick();
            frsky_rx_toggle_ant();
            rx_state = STATE_WILL_RX;
          }
          break;
          /* end STATE_SEARCH */
      }
      break;
      /* end STATE_RXING */
      
    case STATE_RXD:
      /* lastPktTime and lastHopTime are both updated with the current time. */
      /* nextPktTime is updated to reflect the next anticipated packet, in 9 or 18ms. */
      lastPktTime = time;
      lastHopTime = time;
      hopchan = (rxmsg.packet[3] + 1) % D8_NUM_HOP_CHANNELS;
      frsky_state = (rxmsg.packet[3] + 1) % NUM_FRSKY_STATES; // sets up the next state
      fail_state = STATE_RUNNING;
      failsafeStatus = 0;
      HAL_GPIO_WritePin(LED_RED_GPIO, LED_RED_PIN, LED_RED_STATE_OFF);
      HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, LED_GREEN_STATE_ON);
    
      frsky_rx_toggle_ant();
    
      frsky_rx_process_channels();
    
      /* Read and process rssi. Do this after channels! */
      rssi = CC2500_ReadReg(CC2500_REG_RSSI);
//      rssidbm = frsky_getrssidbm(rssi);
      if(rssichan != -1)
      {
        /* turn 0-255 into 988-2012 */
        channelValues[rssichan] = ((uint16_t)rssi * 4) + 988; // TODO: fix ranges
      }
      
      /* read frequency drift estimation and write this value back to the cc2500 offset register. */
      freqoff_est = CC2500_ReadReg(CC2500_REG_FREQEST);
      CC2500_WriteReg(CC2500_REG_FSCTRL0, freqoff_est);
    
      /* nextPktTime is the expected time of the next incoming packet
       * it is either 9ms or 18ms depending on if there is telem txing this cycle. */
      if (frsky_state == FRSKY_TX_TELEM)
      {
        nextPktTime = lastPktTime + 18;
        rx_state = STATE_PREPARE_TX;
      } else {
        nextPktTime = lastPktTime + 9;
        rx_state = STATE_WILL_RX;
      }
      if (gotFirstPkt == 0)
        gotFirstPkt = 1;
      break;
      /* end STATE_RXD */
      
    case STATE_PREPARE_TX:
      frsky2way_d8_build_telem_packet();
      frsky_set_hopchan(hopchan);
      txStartTime = lastHopTime + 2; // set up time to activate txing.
      lastHopTime = lastHopTime + 9; // not actually lasthoptime, but keeps 9ms interval for simplicity.
      rx_state = STATE_WILL_TX;
      break;
      /* end STATE_PREPARE_TX */
    
    case STATE_WILL_TX:
      /* TODO: disable-able telem txing in config. why? cuz. */
      if (time > txStartTime)
      {
        CC2500_WriteTXData(txmsg.packet, txmsg.packet[0]+1);
        rx_state = STATE_TXING;
      }
      break;
      /* end STATE_WILL_TX */
    
    case STATE_TXING:
      if (time > (txStartTime + 8)) // need to give enough time to tx entire packet.
        rx_state = STATE_TXD;
      break;
      /* end STATE_TXING */
      
    case STATE_TXD:
      hopchan = (hopchan + 1) % D8_NUM_HOP_CHANNELS;
      frsky_state = FRSKY_RX1;
      rx_state = STATE_WILL_RX;
      break;
      /* end STATE_TXD */
    
    default:
      rx_state = STATE_SEARCH;
      break;
  }
  /* end rx_state switch statement */
  
  /* stuff to do once after first packet RXd, is there anything? TODO: */
   if (gotFirstPkt == 1)
  {
    gotFirstPkt = 2;
  }
  
  /* detect and handle failsafe condition */
  if ((gotFirstPkt >= 1) & ((time - lastPktTime) > m_config.failsafeDelay) & (failsafeStatus == 0))
  {
    failsafeStatus = 1;
    uint8_t failtype;
    if (m_config.failsafeType == FAILSAFE_AUTODETECT)
    {
      if (m_config.failsafeValues[0] == 0) // failsafe values have not been set.
        failtype = FAILSAFE_STOPOUTPUT;
      else
        failtype = FAILSAFE_SAVEDVALUES;
    } else {
      failtype = m_config.failsafeType;
    }
    
    switch (failtype)
    {
      default:
      case FAILSAFE_STOPOUTPUT:
        output_handle_functions.outputStop();
        break;
      case FAILSAFE_SAVEDVALUES:
        for (uint8_t i = 0; i < MAX_CHANNEL_OUTPUTS; i++)
        {
          channelValues[i] = m_config.failsafeValues[i];
        }
        if(rssichan != -1)
        {
          channelValues[rssichan] = 988; // TODO: fix ranges
        }
        output_handle_functions.outputUpdate();
        break;
    }
  }
  
  /* failsafe button handler */
  /* we have a packet, we're not in failsafe, and we haven't already set failsafes this btnpress. */
  if ((gotFirstPkt >= 1) & (failsafeStatus == 0) & (failsafeBtnSettingStatus == 0))
  {
    if ((HAL_GPIO_ReadPin(BIND_FAIL_BUTTON_GPIO, BIND_FAIL_BUTTON_PIN) == GPIO_PIN_RESET) & (failsafeBtnTime == 0)) // pin is NO and pulled high
    {
      failsafeBtnTime = time;
    }
    else if ((HAL_GPIO_ReadPin(BIND_FAIL_BUTTON_GPIO, BIND_FAIL_BUTTON_PIN) == GPIO_PIN_RESET) & ((time - failsafeBtnTime) > 2000))
    {
      HAL_GPIO_WritePin(LED_RED_GPIO, LED_RED_PIN, LED_RED_STATE_ON);

      failsafeBtnSettingStatus = 1;
      for (uint8_t i = 0; i < MAX_CHANNEL_OUTPUTS; i++)
      {
        m_config.failsafeValues[i] = channelValues[i];
      }
      eeprom_write();
      
      HAL_Delay(1000);
      HAL_GPIO_WritePin(LED_RED_GPIO, LED_RED_PIN, LED_RED_STATE_OFF);
    }
  }
  
  /* Reset failsafe button timer if button is released */
  if (HAL_GPIO_ReadPin(BIND_FAIL_BUTTON_GPIO, BIND_FAIL_BUTTON_PIN) == GPIO_PIN_SET)
  {
    failsafeBtnTime = 0;
    failsafeBtnSettingStatus = 0;
  }
}
