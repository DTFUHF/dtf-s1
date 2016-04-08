
#include "board.h"

/* TODO: put these all together in outputs.h/c */
enum {
  SBUS_STATE_START = 0,
  SBUS_STATE_STOPPED,
  NUM_SBUS_STATES
};

/* Note: using 'baseflight' endianness, not 'mbed' endianness */
#define SBUS_CH_17_MASK    0x01
#define SBUS_CH_18_MASK    0x02
#define SBUS_LOST_PKT_MASK 0x04
#define SBUS_FAILSAFE_MASK 0x08

struct sbus_chans {
  uint8_t startbyte;
  unsigned int chan01 : 11;
  unsigned int chan02 : 11;
  unsigned int chan03 : 11;
  unsigned int chan04 : 11;
  unsigned int chan05 : 11;
  unsigned int chan06 : 11;
  unsigned int chan07 : 11;
  unsigned int chan08 : 11;
  unsigned int chan09 : 11;
  unsigned int chan10 : 11;
  unsigned int chan11 : 11;
  unsigned int chan12 : 11;
  unsigned int chan13 : 11;
  unsigned int chan14 : 11;
  unsigned int chan15 : 11;
  unsigned int chan16 : 11; // 22 bytes of channels
  uint8_t flags;
  uint8_t stopbyte;
} __attribute__ ((__packed__));

extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart1;

extern volatile uint16_t channelValues[MAX_CHANNEL_OUTPUTS];
extern uint8_t failsafeStatus;

static volatile uint8_t outputBuf[25];
static uint8_t sbusStop = SBUS_STATE_STOPPED;
static volatile union {
  uint8_t workingBuf[25];
  struct sbus_chans sbus_chans;
} sbus_data;

void init_sbus(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  TIM_OC_InitTypeDef TIM_OCInitStructure;
  
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  __HAL_RCC_USART1_CLK_ENABLE();
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 100000;
  huart1.Init.WordLength = UART_WORDLENGTH_9B; // including parity.
  huart1.Init.StopBits = UART_STOPBITS_2;
  huart1.Init.Parity = UART_PARITY_EVEN;
  huart1.Init.Mode = UART_MODE_TX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_TXINVERT_INIT;
  huart1.AdvancedInit.TxPinLevelInvert = UART_ADVFEATURE_TXINV_ENABLE;
  
  HAL_UART_Init(&huart1);
  
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = (uint32_t)(HAL_RCC_GetSysClockFreq() / 1000000) - 1; // timer runs at 1MHz
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 7000; // SBUS sends frames every 7ms.
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

void sbus_stop(void)
{
  sbusStop = SBUS_STATE_STOPPED;
}

uint16_t sbus_transform(uint32_t c)
{
  return (uint16_t)(((c - 879) * 1000000) / 624771);
}

void sbus_update(void)
{
  sbusStop = SBUS_STATE_START;
  sbus_data.sbus_chans.startbyte = 0x0F;
  sbus_data.sbus_chans.chan01 = sbus_transform(channelValues[0]);
  sbus_data.sbus_chans.chan02 = sbus_transform(channelValues[1]);
  sbus_data.sbus_chans.chan03 = sbus_transform(channelValues[2]);
  sbus_data.sbus_chans.chan04 = sbus_transform(channelValues[3]);
  sbus_data.sbus_chans.chan05 = sbus_transform(channelValues[4]);
  sbus_data.sbus_chans.chan06 = sbus_transform(channelValues[5]);
  sbus_data.sbus_chans.chan07 = sbus_transform(channelValues[6]);
  sbus_data.sbus_chans.chan08 = sbus_transform(channelValues[7]);
  sbus_data.sbus_chans.chan09 = sbus_transform(channelValues[8]);
  sbus_data.sbus_chans.chan10 = sbus_transform(channelValues[9]);
  sbus_data.sbus_chans.chan11 = sbus_transform(channelValues[10]);
  sbus_data.sbus_chans.chan12 = sbus_transform(channelValues[11]);
  sbus_data.sbus_chans.chan13 = sbus_transform(channelValues[12]);
  sbus_data.sbus_chans.chan14 = sbus_transform(channelValues[13]);
  sbus_data.sbus_chans.chan15 = sbus_transform(channelValues[14]);
  sbus_data.sbus_chans.chan16 = sbus_transform(channelValues[15]);
  sbus_data.sbus_chans.flags = 0x00 | (failsafeStatus ? SBUS_FAILSAFE_MASK : 0x00); // TODO: digital channels
  sbus_data.sbus_chans.stopbyte = 0x00;
  
  __disable_irq(); // TODO: check if DMA is still active
  for (uint8_t i = 0; i < 25; i++)
  {
    outputBuf[i] = sbus_data.workingBuf[i];
  }
  __enable_irq();
}

void sbus_HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *  htim)
{
  if (sbusStop == SBUS_STATE_START)
  {
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)outputBuf, 25);
  }
}
