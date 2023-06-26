/*
 * main.c
 *
 *  Created on: 7 May 2023
 *      Author: Gehad Ahmad
 */

#include "Gpio.h"
#include "Rcc.h"
#include "Std_Types.h"
#include "Utils.h"
#include "Bit_Operations.h"
#include "GPT.h"

#define AMBIENT_LIGHT 2
#define HAZARD_LIGHT 3
#define LOCK_LIGHT 4

#define LOCKED_STATE 0
#define UNLOCKED_STATE 1

#define OFF_STATE 2
#define SYSTEM_ON 1
#define SYSTEM_OFF 0

uint8 handlerPressed = 0;
uint8 doorState;
uint8 doorButton = 0;
uint8 doorButton2 = 0;
uint8 doorOpen = 0;
uint32 timeElapsed = 0;
uint8 caseFlag = 0;
uint8 tenSecflag = 0;
unsigned char safe = 0;
unsigned char firstTimePress = 0;
void case2();
void case4();

int main(){
	Rcc_Init();
	uint8 system = 0;
	RCC_CFGR |= (0x0B << 4);
	Rcc_Enable(RCC_GPIOB);
	Rcc_Enable(RCC_SYSCFG);
	GPIO_Init();
	GPT_Init();
	while (1)
	{
		if(system == SYSTEM_OFF){
			system = SYSTEM_ON;
			doorState = LOCKED_STATE;
			doorOpen = 0;
			GPIO_WritePinValue(HAZARD_LIGHT, LOW);
			GPIO_WritePinValue(LOCK_LIGHT, LOW);
			GPIO_WritePinValue(AMBIENT_LIGHT, LOW);

		}

		handlerPressed = GPIO_ReadPinState(0);
		doorButton = GPIO_ReadPinState(1);
	    if ((handlerPressed == 1) && (doorState == LOCKED_STATE))
	    {
			doorState = UNLOCKED_STATE;
			GPIO_WritePinValue(LOCK_LIGHT, HIGH);
			GPIO_WritePinValue(AMBIENT_LIGHT, HIGH);
			GPT_StartTimer(2000);  
	    	caseFlag = 2;
	    	doorOpen = 0;
		}
		else if((doorState == UNLOCKED_STATE) && (doorButton == 1) && (doorOpen == 0)){
			doorOpen = 1;
			GPIO_WritePinValue(AMBIENT_LIGHT, HIGH);
		}
		else if(safe){
			GPT_StartTimer(8000);
			tenSecflag = 1;
		}

		if(tenSecflag){
			if(!GPT_CheckTimeIsElapsed()){
				doorButton = GPIO_ReadPinState(1);
			}else{
				doorButton2 = GPIO_ReadPinState(1);
				if (((doorButton == doorButton2) && (doorButton == 0)) && (doorState == UNLOCKED_STATE) && (doorOpen == 0)){
					doorState = LOCKED_STATE;
					GPIO_WritePinValue(AMBIENT_LIGHT, LOW);
					GPIO_WritePinValue(LOCK_LIGHT, LOW);
					GPT_StartTimer(2000);
					caseFlag = 4;
				}
			}
			safe = 0;
		}


		if(caseFlag == 2){
			case2();
		}
		else if(caseFlag == 4){
			case4();
		}

	}
	return 0;
}


void case2(){
	if (!GPT_CheckTimeIsElapsed())
	{
		timeElapsed = GPT_GetElapsedTime();
		if ((timeElapsed <= 500)){  // 0.5s
			GPIO_WritePinValue(HAZARD_LIGHT, HIGH);
		}
		else{
			GPIO_WritePinValue(HAZARD_LIGHT, LOW);
		}
	}else{
		GPIO_WritePinValue(AMBIENT_LIGHT, LOW);
		caseFlag = 0;
		safe = 1;
	}
}

void case4(){
	if (!GPT_CheckTimeIsElapsed())
	{
		timeElapsed = GPT_GetElapsedTime();
		if ((timeElapsed < 500) || ((timeElapsed > 1000) && (timeElapsed < 1500))){  
			GPIO_WritePinValue(HAZARD_LIGHT, HIGH);
		}
		else{
			GPIO_WritePinValue(HAZARD_LIGHT, LOW);
		}
	}
	else{
		caseFlag = 0;
		tenSecflag = 0;
	}
}

