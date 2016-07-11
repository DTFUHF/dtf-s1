
#ifndef _BOARD_H_
#define _BOARD_H_

#include "stm32f0xx_hal.h"

#include "drv_spi.h"
#include "cc2500.h"
#include "frsky_rx.h"
#include "frsky_d8_rx.h"
#include "config.h"
#include "rc_output.h"
#include "drv_ppm.h"
#include "drv_sumd.h"
#include "drv_sbus.h"

#if TX_CARRIER_TEST == 1
void tx_carrier_test(void);
#endif

static const char timestamp[] = __DATE__ __TIME__;

// pin definitions
// internal
#define CC2500_SPI_NSS_GPIO  GPIOB
#define CC2500_SPI_NSS_PIN   GPIO_PIN_12
#define CC2500_SPI_SCK_GPIO  GPIOB
#define CC2500_SPI_SCK_PIN   GPIO_PIN_3
#define CC2500_SPI_MISO_GPIO GPIOB
#define CC2500_SPI_MISO_PIN  GPIO_PIN_4
#define CC2500_SPI_MOSI_GPIO GPIOB
#define CC2500_SPI_MOSI_PIN  GPIO_PIN_5
#define CC2500_GD0_GPIO      GPIOA
#define CC2500_GD0_PIN       GPIO_PIN_8
#define CC2500_GD2_GPIO      GPIOA
#define CC2500_GD2_PIN       GPIO_PIN_10
#define CC2500_ANT_SEL_GPIO  GPIOC
#define CC2500_ANT_SEL_PIN   GPIO_PIN_13
#define CC2500_RX_EN_GPIO    GPIOC
#define CC2500_RX_EN_PIN     GPIO_PIN_14
#define CC2500_TX_EN_GPIO    GPIOC
#define CC2500_TX_EN_PIN     GPIO_PIN_15

// leds
#define LED_RED_GPIO        GPIOA
#define LED_RED_PIN         GPIO_PIN_0
#define LED_GREEN_GPIO      GPIOA
#define LED_GREEN_PIN       GPIO_PIN_1
#define LED_RED_STATE_ON    GPIO_PIN_RESET
#define LED_RED_STATE_OFF   GPIO_PIN_SET
#define LED_GREEN_STATE_ON  GPIO_PIN_RESET
#define LED_GREEN_STATE_OFF GPIO_PIN_SET

// inputs
#define SBUS_EN_GPIO          GPIOB
#define SBUS_EN_PIN           GPIO_PIN_0
#define SUMD_EN_GPIO          GPIOB
#define SUMD_EN_PIN           GPIO_PIN_1
#define BIND_FAIL_BUTTON_GPIO GPIOB
#define BIND_FAIL_BUTTON_PIN  GPIO_PIN_2

extern master_config_t m_config;

extern volatile uint16_t channelValues[MAX_CHANNEL_OUTPUTS];

#endif
