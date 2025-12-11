/*----------------------------------------------------------------------------
 * Name:    usbmain.h
 * Purpose: USB Audio Class Demo Header File
 *----------------------------------------------------------------------------*/

#ifndef USBMAIN_H
#define USBMAIN_H

#include "LPC17xx.h"  /* LPC17xx definitions */

/* External Variables */
extern uint8_t  Mute;          /* Mute State */
extern uint32_t Volume;        /* Volume Level */
extern uint32_t *InfoBuf;
extern short *DataBuf;
extern uint16_t DataOut;       /* Data Out Index */
extern uint16_t DataIn;        /* Data In Index */
extern uint8_t DataRun;        /* Data Stream Run State */
extern uint16_t PotVal;        /* Potentiometer Value */
extern uint32_t VUM;           /* VU Meter */
extern uint32_t Tick;          /* Time Tick */
extern volatile uint32_t pclkdiv, pclk;

/* Function Prototypes */
void get_potval(void);
void TIMER0_IRQHandler(void);
void stream_music(void);

#endif /* USBMAIN_H */