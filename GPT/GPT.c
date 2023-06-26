#include "GPT.h"
#include "Rcc.h"
#include "Bit_Operations.h"
#include "Gpio.h"

uint16 called = 0 ;
uint32 remainingTime = 0;
uint32 elapsedTime = 0;


void GPT_Init(void){
	Rcc_Enable(RCC_TIM2);
	TIM2-> ARR = 0xffff;
	TIM2->PSC = 1000-1;  
	SET_BIT(TIM2->EGR, 0);
}


void GPT_StartTimer(uint32 OverFlowTicks){
	CLEAR_BIT(TIM2->CR1, CR1_CEN);
	CLEAR_BIT(TIM2->SR, 0);
	TIM2->CNT = 0;
	TIM2->ARR = OverFlowTicks;
	SET_BIT(TIM2->CR1, 0);
	called = 1;
}

unsigned char GPT_CheckTimeIsElapsed(void){
	if((TIM2->SR & (1 << 0)) != 0){
		TIM2->SR &=~ (1 <<0);
		return 1;
	}
	else{
		return 0;
	}
}

uint32 GPT_GetElapsedTime(void){    
	if(called){
		if(((TIM2->SR & (1 << 0)) != 0)){
			return 0xffffffff;
		}
		else{
			elapsedTime = TIM2->CNT;
			return elapsedTime;
		}
		called = 0;
	}
	else{
		return 0;
	}
}

uint32 GPT_GetRemainingTime(void){   
	if(called){
		if(((TIM2->SR & (1 << 0)) != 0)){
			return 0;
		}
		else{
			return  TIM2->ARR - TIM2->CNT;
		}
	}
	else{
		return 0xffffffff;
	}
}

