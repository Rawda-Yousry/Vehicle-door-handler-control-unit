#include "GPT.h"
#include "Rcc.h"
#include "Bit_Operations.h"
#include "Gpio.h"

uint16 called = 0 ;
uint32 elapsedTime = 0;


void GPT_Init(void){
	Rcc_Enable(RCC_TIM2); // Clock enable on TIM2
	TIM2-> ARR = 0xffff-1;
	TIM2->PSC = 1000-1;
	SET_BIT(TIM2->EGR, EGR_UG); // Set Update generation bit
}


void GPT_StartTimer(uint32 OverFlowTicks){
	CLEAR_BIT(TIM2->CR1, CR1_CEN); // Disable timer
	CLEAR_BIT(TIM2->SR, SR_UIF); // Clear UIF bit
	TIM2->CNT = 0;	// intialize the counter
	TIM2->ARR = OverFlowTicks-1; // update ARR
	SET_BIT(TIM2->CR1, CR1_CEN); // Enable Timer
	called = 1; // set flag to know that timer is called
}

unsigned char GPT_CheckTimeIsElapsed(void){
	if((TIM2->SR & (1 << SR_UIF)) != 0){ // if flag is set then overflow occured
		CLEAR_BIT(TIM2->SR, SR_UIF);
		return 1;
	}
	else{
		return 0;
	}
}

uint32 GPT_GetElapsedTime(void){
	if(called){
		if(((TIM2->SR & (1 << SR_UIF)) != 0)){ // if flag is set and timer was called then it means overflow and 0xffffffff should be returned
			called = 0;
			return 0xffffffff;
		}
		else{		// then the elapsed time is the value in counter register
			elapsedTime = TIM2->CNT;
			return elapsedTime;
		}
	}
	else{
		return 0;
	}
}

uint32 GPT_GetRemainingTime(void){
	if(called){
		if(((TIM2->SR & (1 << SR_UIF)) != 0)){ // if flag is set from last startTimer call then 0 should be returned
			called = 0;
			return 0;
		}
		else{
			return  TIM2->ARR - TIM2->CNT; // Remaining Time = the value in the ARR( the final time ) - the value in the counter register (elapsed Time)
		}
	}
	else{
		return 0xffffffff;
	}
}


