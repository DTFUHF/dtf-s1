
#ifndef _FRSKY2WAY_H_
#define _FRSKY2WAY_H_

#include "board.h"
#include "cc2500.h"


enum {
  BIND_TYPE_NONE = 0,
  BIND_TYPE_D8,
  BIND_TYPE_D16,
  NUM_BIND_TYPES
};

enum {
  RSSI_INJECTION_AUTO = 0,
  RSSI_INJECTION_NONE,
  RSSI_INJECTION_CHAN,
  NUM_RSSI_INJECTION_TYPES
};

// common functions for tx/rx

void frsky_bind_cal(void);
void frsky_full_cal(void);
void frsky_set_hopchan(uint8_t);
void frsky_set_chan(uint8_t);
void frsky_rx_enable(uint8_t);

/******************* exported RX functions ************************/
/******************* common ***************************************/
void frsky_rx_select_ant(uint8_t ant);
void frsky_rx_toggle_ant(void);
int16_t frsky_getrssidbm(uint8_t);

#endif
