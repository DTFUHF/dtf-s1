
#ifndef _DRV_CONFIG_H_
#define _DRV_CONFIG_H_

#include "board.h"
#include "cc2500.h"
#include "rc_output.h"

// main config struct for eeprom
#define EEPROM_VERSION 1
#define MASTER_CONFIG_CRC_POLY 0x1021
typedef struct master_config_t {
  uint16_t eeprom_version;
  struct frsky2way_bind_info_s frsky2way_bind_info;
  uint8_t output_type;
  uint16_t failsafeValues[MAX_CHANNEL_OUTPUTS];
  uint8_t failsafeType;
  uint16_t failsafeDelay;
  uint8_t rssi_injection_type;
  uint8_t rssi_injection_chan; // zero-indexed. 0-31 range
  /* CRC is always last. */
  uint16_t crc;
} master_config_t;

void eeprom_init_load(void);
void eeprom_read(void);
void eeprom_write(void);
uint8_t config_validate(void);
void config_defaults(void);

#endif
