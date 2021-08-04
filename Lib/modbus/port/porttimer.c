/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"


/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{

    if(usTim1Timerout50us < 2){
        usTim1Timerout50us = 2;
    }

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->PSC = (5000-1);
    TIM2->CNT = 0;
    TIM2->ARR = (usTim1Timerout50us-1);

    TIM2->EGR |= TIM_EGR_UG;
    while(!(TIM2->SR & TIM_SR_UIF)){};
    TIM2->SR = 0;

	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM2_IRQn); 
    return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
}

inline void
vMBPortTimersDisable(  )
{
    TIM2->CR1 &= ~TIM_CR1_CEN;
    TIM2->CNT = 0;
    /* Disable any pending timers. */
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */

void TIM2_IRQHandler(void){
	TIM2->SR &= ~TIM_SR_UIF;
    //DEBUG("TIM");
    (void)pxMBPortCBTimerExpired();
}
