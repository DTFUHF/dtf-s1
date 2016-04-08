
#include "board.h"
#include "crc.h"

#define SUMD_CRC_POLY 0x1021

enum {
  SUMD_STATE_START = 0,
  SUMD_STATE_STOPPED,
  NUM_SUMD_STATES
};

extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart1;

extern volatile uint16_t channelValues[MAX_CHANNEL_OUTPUTS];
extern uint8_t num_frsky2way_channels; // 1-indexed
extern uint8_t failsafeStatus;

static volatile uint8_t workingBuf[(MAX_CHANNEL_OUTPUTS*2) + 5];
static volatile uint8_t outputBuf[(MAX_CHANNEL_OUTPUTS*2) + 5];
static uint8_t numChannels = 0;
static uint8_t sumdStop = SUMD_STATE_STOPPED;

void init_sumd(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  TIM_OC_InitTypeDef TIM_OCInitStructure;
  
  numChannels = num_frsky2way_channels;
  
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  __HAL_RCC_USART1_CLK_ENABLE();
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&huart1);
  
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = (uint32_t)(HAL_RCC_GetSysClockFreq() / 1000000) - 1; // timer runs at 1MHz
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 10000; // SUMD sends frames every 10ms.
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  HAL_TIM_OC_Init(&htim1);
  
  TIM_OCInitStructure.OCMode = TIM_OCMODE_TIMING;
  TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_LOW;
  TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_SET;
  TIM_OCInitStructure.Pulse = 0;
  HAL_TIM_OC_ConfigChannel(&htim1, &TIM_OCInitStructure, TIM_CHANNEL_2);
  HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_2);
}

void sumd_eeprom_crc_init(void)
{
  hcrc.Instance = CRC;
  HAL_CRC_DeInit(&hcrc);
  
  CRC_InitTypeDef crci;
  crci.CRCLength = CRC_POLYLENGTH_16B;
  crci.DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE;
  crci.InitValue = 0x00000000;
  crci.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
  crci.GeneratingPolynomial = SUMD_CRC_POLY;
  crci.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  crci.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  hcrc.Init = crci;
  HAL_CRC_Init(&hcrc);
}

void sumd_stop(void)
{
  sumdStop = SUMD_STATE_STOPPED;
}

void sumd_update(void)
{
  // TODO: clean this up, don't init the header every time, dummy.
  sumdStop = SUMD_STATE_START;
  sumd_eeprom_crc_init(); // TODO: static init struct
  workingBuf[0] = 0xA8;
  workingBuf[1] = (failsafeStatus ? 0x81 : 0x01);
  workingBuf[2] = numChannels;
  for (uint8_t i = 1; i < numChannels + 1; i++)
  {
    uint16_t tmp = (uint16_t)(channelValues[i - 1] << 3);
    workingBuf[i * 2 + 1] = (uint8_t)(tmp >> 8);
    workingBuf[i * 2 + 2] = (uint8_t)(tmp & 0xFF);
  }
  
  uint16_t crc = (uint16_t)HAL_CRC_Calculate(&hcrc, (uint32_t *)&workingBuf, numChannels * 2 + 3);
  workingBuf[(numChannels + 1) * 2 + 1] = (uint8_t)(crc >> 8);
  workingBuf[(numChannels + 1) * 2 + 2] = (uint8_t)(crc & 0xFF);
  
  __disable_irq(); // TODO: check if DMA is still active
  for (uint8_t i = 0; i < (numChannels * 2 + 5); i++)
  {
    outputBuf[i] = workingBuf[i];
  }
  __enable_irq();
}

void sumd_HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *  htim)
{
  if (sumdStop == SUMD_STATE_START)
  {
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)outputBuf, numChannels * 2 + 5);
  }
}
