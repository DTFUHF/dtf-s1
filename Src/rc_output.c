
#include "board.h"

// TODO: no outputs til first packet rxd

volatile uint16_t channel_output_buffer[MAX_CHANNEL_OUTPUTS] = {0};

output_handle_functions_t output_handle_functions;
static uint8_t type;

void init_output(void)
{
  
  switch(m_config.output_type)
  {
    case OUTPUT_AUTO:
      if (HAL_GPIO_ReadPin(SBUS_EN_GPIO, SBUS_EN_PIN) == GPIO_PIN_RESET)
      {
        type = OUTPUT_SBUS;
      } 
      else if (HAL_GPIO_ReadPin(SUMD_EN_GPIO, SUMD_EN_PIN) == GPIO_PIN_RESET)
      {
        type = OUTPUT_SUMD;
      }
      else
      {
        type = OUTPUT_PPM;
      }
    break;
    
    default:
    case OUTPUT_PPM:
      type = OUTPUT_PPM;
    break;
    
    case OUTPUT_SBUS:
      type = OUTPUT_SBUS;
    break;
    
    case OUTPUT_SUMD:
      type = OUTPUT_SUMD;
    break;
  }
  
  switch (type)
  {
    default:
    case OUTPUT_PPM:
      init_ppm();
      output_handle_functions.outputStop = ppm_stop;
      output_handle_functions.outputUpdate = ppm_update;
    break;
    
    case OUTPUT_SBUS:
      init_sbus();
      output_handle_functions.outputStop = sbus_stop;
      output_handle_functions.outputUpdate = sbus_update;
    break;
    
    case OUTPUT_SUMD:
      init_sumd();
      output_handle_functions.outputStop = sumd_stop;
      output_handle_functions.outputUpdate = sumd_update;
    break;
  }
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *  htim)
{
  switch (type)
  {
    default:
    case OUTPUT_PPM:
      ppm_HAL_TIM_OC_DelayElapsedCallback(htim);
      break;
    case OUTPUT_SBUS:
      sbus_HAL_TIM_OC_DelayElapsedCallback(htim);
      break;
    case OUTPUT_SUMD:
      sumd_HAL_TIM_OC_DelayElapsedCallback(htim);
      break;
  }
}
