
#ifndef _DRV_SPI_H_
#define _DRV_SPI_H_

#include "board.h"

void spi1Select(void);
void spi1Unselect(void);
void initSpi1(void);
uint8_t spi1_txrx_byte(uint8_t byte);
//uint8_t spi1_rx_byte(void);
//void spi1_txrx_no_nss(uint8_t * tx, uint8_t * rx, uint8_t length);
void spi1_tx(uint8_t * tx, uint8_t length);
void spi1_rx(uint8_t * rx, uint8_t length);
//void spi1_txrx(uint8_t * tx, uint8_t * rx, uint8_t length);

//extern SPI_HandleTypeDef hspi1;

#endif
