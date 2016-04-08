
#ifndef _DRV_SUMD_H_
#define _DRV_SUMD_H_

#include "board.h"

void sumd_HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *  htim);
void init_sumd(void);
void sumd_stop(void);
void sumd_update(void);

#endif
