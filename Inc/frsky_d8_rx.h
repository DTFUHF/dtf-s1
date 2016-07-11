
#ifndef _FRSKY_D8_RX_H_
#define _FRSKY_D8_RX_H_

#include "board.h"
#include "cc2500.h"

/******************* constants ************************************/
#define D8_NUM_HOP_CHANNELS 47 // sized for d8 no clue about others
#define D8_FRSKY_BIND_CHANNEL 0
#define D8_MAX_PACKET_SIZE 22 // only need 17? or 20?

/******************* exported RX D8 functions *********************/
void frsky_d8_rx_init(void);
void frsky_d8_rx_startup(void);
/******************* binding **************************************/
void frsky_d8_rx_bind_init(void);
uint8_t frsky_d8_rx_binding(void);
/******************* normal ***************************************/
void frsky_d8_rx_normal_init(void);
void frsky_d8_rx_loop(void);

/******************* d8 init array ********************************/
#define D8_RX_CC2500_INIT_ARRAY_SIZE 18
static const uint8_t d8_rx_initArray[D8_RX_CC2500_INIT_ARRAY_SIZE][2] = 
{
  {CC2500_REG_MCSM1,    0x0c}, // always clear channel, stay in RX when got pkt, go to idle after tx
  {CC2500_REG_MCSM0,    0x08}, // autocal never, med startup time, idle->tx/rx time with cal is 809us, without is 88.4us
  {CC2500_REG_PKTLEN,   0x19}, // fixed/max pkt length = 25
  {CC2500_REG_PKTCTRL1, 0x0D}, // enable tx/rx of address
  {CC2500_REG_PKTCTRL0, 0x05}, // whitening off, fifos on, crc on, variable pkt length
  {CC2500_REG_PATABLE,  0xff}, // max powahhh
  {CC2500_REG_FSCTRL1,  0x08}, // IF ~= 203kHz
  {CC2500_REG_FSCTRL0,  0x00}, // fine frequency offset, ~1.6kHz step
  {CC2500_REG_FREQ2,    0x5c}, // upper byte of FREQ[23:0]
  {CC2500_REG_FREQ1,    0x76}, // FREQ[15:8]
  {CC2500_REG_FREQ0,    0x27}, // FREQ[7:0] default = 6059559 -> 2 403 999 847Hz
  {CC2500_REG_MDMCFG4,  0xaa}, // decimation/channel bw = 135417kHz also symbolrate exponent
  {CC2500_REG_MDMCFG3,  0x39}, // symbolrate = 56435
  {CC2500_REG_MDMCFG2,  0x11}, // dc filter disabled, gfsk, no manchester, 15/16 sync bits
  {CC2500_REG_MDMCFG1,  0x23}, // no fec, 4 preamble bytes, channel spacing exponent
  {CC2500_REG_MDMCFG0,  0x7a}, // chan spacing = 48657khz
  {CC2500_REG_DEVIATN,  0x42}, // freq deviation for gfsk 31738kHz +-
  {CC2500_REG_FOCCFG,   0x16}, // freq offset gating disable, enable clock recovery right away, freq compensation loop default
};

/******************* RX packet structs and unions *****************/
/******************* this is saved in eeprom **********************/
struct frsky_d8_bind_info_s {
  uint8_t bind_type;
  uint16_t bind_id;
  uint8_t hopChannels[D8_NUM_HOP_CHANNELS];
};

/******************* these are stored in ram for receiving pkts ***/
struct d8_bind_packet_data_s {
  uint8_t len;
  uint16_t header1;
  uint16_t header2;
} __attribute__ ((__packed__));

struct d8_rx_packet_data_s {
  uint8_t len;
  uint16_t header_bindcode;
  uint8_t header_frsky_rx_state;
  uint16_t header_magic;
  // sadly, c does not allow arrays of bitfields, how can this be better?
  uint8_t ch1_low;
  uint8_t ch2_low;
  uint8_t ch3_low;
  uint8_t ch4_low;
  unsigned int ch1_high : 4;
  unsigned int ch2_high : 4;
  unsigned int ch3_high : 4;
  unsigned int ch4_high : 4;
  uint8_t ch5_low;
  uint8_t ch6_low;
  uint8_t ch7_low;
  uint8_t ch8_low;
  unsigned int ch5_high : 4;
  unsigned int ch6_high : 4;
  unsigned int ch7_high : 4;
  unsigned int ch8_high : 4;
} __attribute__ ((__packed__));

struct d8_telem_packet_data_s {
  uint8_t len;
  uint16_t header_bindcode;
  uint8_t telem_a1;
  uint8_t telem_a2;
  uint8_t telem_rssi;
} __attribute__ ((__packed__));

typedef union {
  uint8_t packet[D8_MAX_PACKET_SIZE];
  struct d8_bind_packet_data_s d8_bind_packet_data;
  struct d8_rx_packet_data_s d8_rx_packet_data;
  struct d8_telem_packet_data_s d8_telem_packet_data;
} packet_union;

#endif // _FRSKY_D8_RX_H_
