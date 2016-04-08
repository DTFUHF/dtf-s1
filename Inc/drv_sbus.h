
#ifndef _DRV_SBUS_H_
#define _DRV_SBUS_H_

#include "board.h"

void sbus_HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *  htim);
void init_sbus(void);
void sbus_stop(void);
void sbus_update(void);

#endif
