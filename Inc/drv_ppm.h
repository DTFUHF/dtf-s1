
#ifndef _DRV_PPM_H_
#define _DRV_PPM_H_

#include "board.h"

void ppm_HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *  htim);
void init_ppm(void);
void ppm_stop(void);
void ppm_update(void);

#endif
