#include "rcc.h"

static volatile uint32_t lock_nesting_count = 0;

void Delay_C(uint32_t nTime)
{
	for(uint32_t i=0; i < (SystemCoreClock / nTime); i++){
		 asm volatile("NOP");
		 };
}

void RCC_init(void){

	RCC->CR |= ((uint32_t)RCC_CR_HSEON);
	while(!(RCC->CR & RCC_CR_HSERDY)){};

	
 	FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
 	FLASH->ACR |= FLASH_ACR_PRFTEN|\
				  FLASH_ACR_LATENCY_3WS|\
				  FLASH_ACR_ICEN|\
				  FLASH_ACR_DCEN;
	
	 RCC->CFGR = RCC_CFGR_HPRE_DIV1|\
			 	 RCC_CFGR_PPRE2_DIV1|\
 				 RCC_CFGR_PPRE1_DIV2;
	
	RCC->PLLCFGR = (RCC_PLL_M << RCC_PLLCFGR_PLLM_Pos)|\
				   (RCC_PLL_N << RCC_PLLCFGR_PLLN_Pos)|\
				   (((RCC_PLL_P >> 1) - 1) << RCC_PLLCFGR_PLLP_Pos)|\
				   RCC_PLL_SOURCE << RCC_PLLCFGR_PLLSRC_Pos|\
				   (RCC_PLL_Q << RCC_PLLCFGR_PLLQ_Pos);
	
	RCC->CR |= RCC_CR_PLLON;

	while((RCC->CR & RCC_CR_PLLRDY) == 0) {};
	
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	
	while ((RCC->CFGR & RCC_CFGR_SWS) != (uint32_t)0x08) {};

	SystemCoreClockUpdate();
	dwt_init();

	INFO("SystemCore %d",SystemCoreClock);
}


void system_reset(void){
	SCB->AIRCR = (0x5FA << SCB_AIRCR_VECTKEYSTAT_Pos) | SCB_AIRCR_SYSRESETREQ_Msk;
	__DSB();                                                                                 
  	while(1);  
}


void __critical_enter(void){
    __disable_irq();
    ++lock_nesting_count;
}

void __critical_exit(void){
    --lock_nesting_count;
    if (lock_nesting_count == 0) {
        __enable_irq();
    }
}



