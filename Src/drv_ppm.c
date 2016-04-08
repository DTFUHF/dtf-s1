
#include "board.h"

extern TIM_HandleTypeDef htim1;

// no buffer needed for interrupt outputs
extern volatile uint16_t channelValues[MAX_CHANNEL_OUTPUTS];

enum {
  PPM_STATE_START = 0,
  PPM_STATE_WILLSTOP,
  PPM_STATE_STOPPED,
  NUM_PPM_STATES
};

volatile uint8_t ppmcount = 0;
static uint8_t numChannels = 0;
extern uint8_t num_frsky2way_channels;
static uint8_t ppmStop = PPM_STATE_STOPPED;
static GPIO_InitTypeDef GPIO_InitStruct;

void init_ppm(void)
{
  TIM_OC_InitTypeDef TIM_OCInitStructure;
  
  numChannels = num_frsky2way_channels;
  
  __TIM1_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); // Idle pin state.
  
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = (uint32_t)(HAL_RCC_GetSysClockFreq() / 1000000) - 1; // timer runs at 1MHz
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000; // default all low, 1ms
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  HAL_TIM_OC_Init(&htim1);
  
  TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
  TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_LOW;
  TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_SET;
  TIM_OCInitStructure.Pulse = 300;
  HAL_TIM_OC_ConfigChannel(&htim1, &TIM_OCInitStructure, TIM_CHANNEL_2);
}

void ppm_start(void)
{
  if (HAL_TIM_OC_GetState(&htim1) != HAL_TIM_STATE_RESET)
  {
    HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_2);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void ppm_stop(void)
{
  ppmStop = PPM_STATE_WILLSTOP;
}

void ppm_update(void)
{
  if(ppmStop == PPM_STATE_STOPPED || ppmStop == PPM_STATE_WILLSTOP)
  {
    ppm_start();
    ppmStop = PPM_STATE_START;
  }
  /* No other actions as the routine uses channelValues directly. */
}

void ppm_HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *  htim)
{
  if(htim->Instance==TIM1)
  {
    if (ppmStop == PPM_STATE_STOPPED)
    {
      __HAL_TIM_SET_AUTORELOAD(htim, 3500);
      return;
    }
    if (ppmcount < numChannels)
    {
      uint16_t ch = channelValues[ppmcount++];
       __HAL_TIM_SET_AUTORELOAD(htim, ch);
    }
    else
    {
      __HAL_TIM_SET_AUTORELOAD(htim, 3500);
      ppmcount = 0;
      /* ensure complete ppm frame when ppm is stopped, by stopping ppm at this stage. */
      if (ppmStop == PPM_STATE_WILLSTOP)
      {
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        HAL_TIM_OC_Stop_IT(&htim1, TIM_CHANNEL_2);
        ppmStop = PPM_STATE_STOPPED;
      }
    }
  }
}
