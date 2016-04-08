
#ifndef _CC2500_H_
#define _CC2500_H_

#include "board.h"

#define CC2500_REG_IOCFG2   0x00
#define CC2500_REG_IOCFG1   0x01
#define CC2500_REG_IOCFG0   0x02
#define CC2500_REG_FIFOTHR  0x03
#define CC2500_REG_SYNC1    0x04
#define CC2500_REG_SYNC0    0x05
#define CC2500_REG_PKTLEN   0x06
#define CC2500_REG_PKTCTRL1 0x07
#define CC2500_REG_PKTCTRL0 0x08
#define CC2500_REG_ADDR     0x09
#define CC2500_REG_CHANNR   0x0A
#define CC2500_REG_FSCTRL1  0x0B
#define CC2500_REG_FSCTRL0  0x0C
#define CC2500_REG_FREQ2    0x0D
#define CC2500_REG_FREQ1    0x0E
#define CC2500_REG_FREQ0    0x0F
#define CC2500_REG_MDMCFG4  0x10
#define CC2500_REG_MDMCFG3  0x11
#define CC2500_REG_MDMCFG2  0x12
#define CC2500_REG_MDMCFG1  0x13
#define CC2500_REG_MDMCFG0  0x14
#define CC2500_REG_DEVIATN  0x15
#define CC2500_REG_MCSM2    0x16
#define CC2500_REG_MCSM1    0x17
#define CC2500_REG_MCSM0    0x18
#define CC2500_REG_FOCCFG   0x19
#define CC2500_REG_BSCFG    0x1A
#define CC2500_REG_AGCCTRL2 0x1B
#define CC2500_REG_AGCCTRL1 0x1C
#define CC2500_REG_AGCCTRL0 0x1D
#define CC2500_REG_WOREVT1  0x1E
#define CC2500_REG_WOREVT0  0x1F
#define CC2500_REG_WORCTRL  0x20
#define CC2500_REG_FREND1   0x21
#define CC2500_REG_FREND0   0x22
#define CC2500_REG_FSCAL3   0x23
#define CC2500_REG_FSCAL2   0x24
#define CC2500_REG_FSCAL1   0x25
#define CC2500_REG_FSCAL0   0x26
#define CC2500_REG_RCCTRL1  0x27
#define CC2500_REG_RCCTRL0  0x28
#define CC2500_REG_FSTEST   0x29
#define CC2500_REG_PTEST    0x2A
#define CC2500_REG_AGCTEST  0x2B
#define CC2500_REG_TEST2    0x2C
#define CC2500_REG_TEST1    0x2D
#define CC2500_REG_TEST0    0x2E

#define CC2500_REG_PARTNUM        0xF0
#define CC2500_REG_VERSION        0xF1
#define CC2500_REG_FREQEST        0xF2
#define CC2500_REG_LQI            0xF3
#define CC2500_REG_RSSI           0xF4
#define CC2500_REG_MARCSTATE      0xF5
#define CC2500_REG_WORTIME1       0xF6
#define CC2500_REG_WORTIME0       0xF7
#define CC2500_REG_PKTSTATUS      0xF8
#define CC2500_REG_VCO_VC_DAC     0xF9
#define CC2500_REG_TXBYTES        0xFA
#define CC2500_REG_RXBYTES        0xFB
#define CC2500_REG_RCCTRL1_STATUS 0xFC
#define CC2500_REG_RCCTRL2_STATUS 0xFD

#define CC2500_REG_PATABLE  0x3E
#define CC2500_REG_TXFIFO   0x3F
#define CC2500_REG_RXFIFO   0x3F
#define CC2500_REG_FIFO     0x3F
 
#define CC2500_WRITE_SINGLE 0x00
#define CC2500_WRITE_BURST  0x40
#define CC2500_READ_SINGLE  0x80
#define CC2500_READ_BURST   0xC0

#define CC2500_STROBE_SRES    0x30
#define CC2500_STROBE_SFSTXON 0x31
#define CC2500_STROBE_SXOFF   0x32
#define CC2500_STROBE_SCAL    0x33
#define CC2500_STROBE_SRX     0x34
#define CC2500_STROBE_STX     0x35
#define CC2500_STROBE_SIDLE   0x36
#define CC2500_STROBE_SAFC    0x37
#define CC2500_STROBE_SWOR    0x38
#define CC2500_STROBE_SPWD    0x39
#define CC2500_STROBE_SFRX    0x3A
#define CC2500_STROBE_SFTX    0x3B
#define CC2500_STROBE_SWORRST 0x3C
#define CC2500_STROBE_SNOP    0x3D

enum TXRX_State {
  TXRX_OFF,
  TX_EN,
  RX_EN,
};

// exported functions
void CC2500_WriteReg(uint8_t address, uint8_t data);
uint8_t CC2500_ReadReg(uint8_t address);
void CC2500_Reset(void);
void CC2500_Strobe(uint8_t cmd);
void CC2500_WriteTXData(uint8_t *packet, uint8_t length);
void CC2500_ReadRXData(uint8_t *packet, uint8_t len);
void CC2500_SetGPIOTxRxMode(enum TXRX_State state);
void CC2500_SetChan(uint8_t chan);

#endif
