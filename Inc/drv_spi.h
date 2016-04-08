
#ifndef _DRV_SPI_H_
#define _DRV_SPI_H_

#include "board.h"

void spi1Select(void);
void spi1Unselect(void);

extern SPI_HandleTypeDef hspi1;

#endif
