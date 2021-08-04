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

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
// static void prvvUARTTxReadyISR( void );
// static void prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
  	//DEBUG("SE RX: %d  TX %d",xRxEnable,xTxEnable);
    //while((USART2->SR & USART_SR_TC) == 0){};

    if(xRxEnable == TRUE){
        USART2->CR1 |= USART_CR1_RXNEIE;
    }else{
        USART2->CR1 &= ~USART_CR1_RXNEIE;
    }

  

	if(xTxEnable == TRUE){
          USART2->CR1 |= USART_CR1_TXEIE;
    }else{
          USART2->CR1 &= ~USART_CR1_TXEIE;
    }



}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    uint32_t parity_mask;
    //PA2 TX  PA3 RX  AF7
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~(GPIO_MODER_MODE2|GPIO_MODER_MODE3);
    GPIOA->MODER |= (GPIO_MODER_MODE2_1|GPIO_MODER_MODE3_1);

    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT2|GPIO_OTYPER_OT3);
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2|GPIO_OSPEEDER_OSPEEDR3;
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD2|GPIO_PUPDR_PUPD3);

    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL2|GPIO_AFRL_AFRL3);
    GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos)|(7 << GPIO_AFRL_AFSEL3_Pos);

    USART2->CR1 = 0;
    
    switch (eParity) {
        case MB_PAR_NONE:
            parity_mask = 0;
            break;
        case MB_PAR_ODD:
            parity_mask = USART_CR1_PCE|USART_CR1_PS;
            break;
        case MB_PAR_EVEN:
            parity_mask = USART_CR1_PCE;
            break;
        
        default:
            parity_mask = 0;
            break;
    }

    USART2->CR1 = parity_mask | USART_CR1_TE | USART_CR1_RE;
    USART2->BRR = (APB1_CLK)/ulBaudRate;
    USART2->CR1 |= USART_CR1_UE;
    
    NVIC_EnableIRQ(USART2_IRQn);
    return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
   	
    while(!(USART2->SR & USART_SR_TXE)) {};  
	USART2->DR  =  ucByte;
    //while((USART2->SR & USART_SR_TC) == 0){};
    //DEBUG("Send %0X",ucByte);
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
    
    *pucByte = (uint8_t)(USART2->DR & (uint8_t)0x00FF);  
   //DEBUG("Get byte %0X",*pucByte);
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
// static void prvvUARTTxReadyISR( void )
// {
//     pxMBFrameCBTransmitterEmpty(  );
// }

// /* Create an interrupt handler for the receive interrupt for your target
//  * processor. This function should then call pxMBFrameCBByteReceived( ). The
//  * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
//  * character.
//  */
// static void prvvUARTRxISR( void )
// {
//     pxMBFrameCBByteReceived(  );
// }


void USART2_IRQHandler(void) {

    if(USART2->CR1 & USART_CR1_RXNEIE){
        if(USART2->SR & USART_SR_RXNE){
            pxMBFrameCBByteReceived();
        }
    }

    if(USART2->CR1 & USART_CR1_TXEIE){
        if(USART2->SR & USART_SR_TXE){
            pxMBFrameCBTransmitterEmpty();
        }
    }


}