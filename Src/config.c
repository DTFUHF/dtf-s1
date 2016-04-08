
#define EEPROM_PAGE_START 0x0801F800 // use last page in f072C8. make sure linker settings dont use this page. TODO: set this dynamically based on chip

#include "board.h"
#include "crc.h"
#include <string.h>

__align(4) master_config_t m_config;

void eeprom_crc_init(void)
{
  hcrc.Instance = CRC;
  HAL_CRC_DeInit(&hcrc);
  
  CRC_InitTypeDef crci; // TODO: static init struct
  crci.CRCLength = CRC_POLYLENGTH_16B;
  crci.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  crci.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
  crci.GeneratingPolynomial = MASTER_CONFIG_CRC_POLY;
  crci.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  crci.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_HALFWORDS;
  hcrc.Init = crci;
  HAL_CRC_Init(&hcrc);
}

void eeprom_init_load(void)
{
  // load eeprom on startup, if not valid, make a new one
  eeprom_read();
  if (config_validate() == 0)
  {
    config_defaults();
    eeprom_write();
  }
}

void eeprom_read(void)
{
  // TODO: error handling
  memcpy(&m_config, (char *)EEPROM_PAGE_START, sizeof(m_config));
}

void eeprom_write(void)
{
  // TODO: include error handling, etc.
  
  eeprom_crc_init();
  m_config.crc = (uint16_t)HAL_CRC_Calculate(&hcrc, (uint32_t *)&m_config, (sizeof(m_config) - sizeof(m_config.crc)) / 2);
  
  FLASH_EraseInitTypeDef flash_erase_typedef;
  flash_erase_typedef.NbPages = 1;
  flash_erase_typedef.PageAddress = EEPROM_PAGE_START;
  flash_erase_typedef.TypeErase = FLASH_TYPEERASE_PAGES;
  
  uint32_t error;
  
  HAL_FLASH_Unlock();
  HAL_FLASHEx_Erase(&flash_erase_typedef, &error);
  for (uint16_t i=0; i < sizeof(m_config); i += 4)
  {
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, EEPROM_PAGE_START + i, *(uint32_t *)((char *)&m_config + i));
  }
  HAL_FLASH_Lock();
}

uint8_t config_validate(void)
{
  if (m_config.eeprom_version != EEPROM_VERSION) {
    return 0;
  }
  eeprom_crc_init();
  if (HAL_CRC_Calculate(&hcrc, (uint32_t *)&m_config, (sizeof(m_config) - sizeof(m_config.crc)) / 2) != m_config.crc) {
    return 0;
  }
  
  return 1;
}

void config_defaults(void)
{
  m_config.eeprom_version = EEPROM_VERSION;
  
  m_config.frsky2way_bind_info.bind_type = BIND_TYPE_NONE;
  m_config.frsky2way_bind_info.bind_id = 0x0000;
  for (uint8_t i = 0; i < NUM_HOP_CHANNELS; i++)
  {
    m_config.frsky2way_bind_info.hopChannels[i] = 0x00;
  }
  
  m_config.output_type = OUTPUT_AUTO;
  
  for (uint8_t i = 0; i < MAX_CHANNEL_OUTPUTS; i++)
  {
    m_config.failsafeValues[i] = 0;
  }
  m_config.failsafeType = FAILSAFE_AUTODETECT;
  m_config.failsafeDelay = 1000;
  
  m_config.rssi_injection_type = RSSI_INJECTION_AUTO;
  m_config.rssi_injection_chan = 0;
}
