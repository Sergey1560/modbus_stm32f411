#include "gpio.h"


void led_init(void){
   	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODE13_0;
	GPIOC->OSPEEDR |= (2 << GPIO_OSPEEDR_OSPEED13_Pos);
	GPIOC->PUPDR |= GPIO_PUPDR_PUPD13_1; //Pull down
}

void btn_init(void){
   	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~GPIO_MODER_MODE0;
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;
    GPIOA->OTYPER |= GPIO_OTYPER_OT0;
}


uint8_t led_state(void){
    //led enable by writing 0

    if(GPIOC->IDR & GPIO_IDR_ID13){
        return 0;
    }else{
        return 1;
    }
}

void led_change(uint8_t value){
    //led enable by writing 0
    if(value){
        GPIOC->BSRR = GPIO_BSRR_BR13;
    }else{
        GPIOC->BSRR = GPIO_BSRR_BS13;
    }
}


uint8_t btn_state(void){

    if(GPIOA->IDR & GPIO_IDR_ID0){
        return 1;
    }else{
        return 0;
    }
}