#include "uptime.h"

static volatile uint16_t uptime;

void uptime_init(void){

  	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    TIM3->PSC = (50000 - 1);
    TIM3->CNT = 0;
    TIM3->ARR = (2000-1);  //2000 - 1sec

    TIM3->EGR |= TIM_EGR_UG;
    while(!(TIM3->SR & TIM_SR_UIF)){};
    TIM3->SR = 0;

	TIM3->DIER |= TIM_DIER_UIE;

	NVIC_EnableIRQ(TIM3_IRQn); 
    uptime = 0;
    TIM3->CR1 |= TIM_CR1_CEN;


};

void TIM3_IRQHandler(void){
	TIM3->SR &= ~TIM_SR_UIF;
    ++uptime;    
}



uint16_t uptime_get_value(void){
    return uptime;
};
