#include "Gpio.h"
#include "Rcc.h"
#include "Std_Types.h"
#include "Utils.h"
#include "Bit_Operations.h"
#include "GPT.h"

// LEDs pins
#define AMBIENT_LIGHT 2
#define HAZARD_LIGHT 3
#define LOCK_LIGHT 4

#define LOCKED_STATE 0
#define UNLOCKED_STATE 1

#define SAFE 1
#define NOT_SAFE 0

#define OPEN 1
#define CLOSE 0

#define SYSTEM_ON 1
#define SYSTEM_OFF 0

// Buttons pins
#define HANDLER_BUTTON 0
#define DOOR_BUTTON 1

// Flags to indicate if timer have already started not start it again
uint8 startTimer = 0;
uint8 startTimer_4 = 0;
uint8 startTimer_6 = 0;

uint8 handlerPressed = 0;
uint8 doorState = LOCKED_STATE;
uint8 doorButton = 0;
uint8 doorButtonState = LOCKED_STATE;
uint8 door = 0;
uint32 timeElapsed = 0;
uint8 caseFlag = 0;
uint8 tenSecflag = 0;

unsigned char safe = 0;
unsigned char firstTimePress = 0;

void case2();
void case3();
void case4();
void case5();
void case6();

int main(){
	Rcc_Init();
	uint8 system = 0;
	RCC_CFGR |= (0x0B << 4); // Reducing RCC internal clock to 1 MHz
	Rcc_Enable(RCC_GPIOB);
	GPIO_Init();
	GPT_Init();
	while (1)
	{
		// Default State
		if(system == SYSTEM_OFF){
			system = SYSTEM_ON;
			doorState = LOCKED_STATE;
			door = CLOSE;
			GPIO_WritePinValue(HAZARD_LIGHT, LOW);
			GPIO_WritePinValue(LOCK_LIGHT, LOW);
			GPIO_WritePinValue(AMBIENT_LIGHT, LOW);

		}
		else{

		// Reading Buttons
		handlerPressed = GPIO_ReadPinState(HANDLER_BUTTON);
		doorButton = GPIO_ReadPinState(DOOR_BUTTON);

		// Toggling button state if pressed and it is unlocked --> make it locked
		// if Pressed and it is locked --> make it unlocked
		if (doorButton == 1) {
			if (doorButtonState == LOCKED_STATE) {
				doorButtonState = UNLOCKED_STATE;
			}
			else {
				doorButtonState = LOCKED_STATE;
			}
		}
		// Second case: If handler pressed and doorState was locked state
		// Ambient light is on for 2 sec then off
		// Lock light is high
		// Hazard light is on for 0.5 sec then off for 0.5 sec (one time blink)
	    if ((handlerPressed == 1) && (doorState == LOCKED_STATE))
	    {
	    	safe = NOT_SAFE;
	    	doorState = UNLOCKED_STATE;
			GPIO_WritePinValue(LOCK_LIGHT, HIGH);
			GPIO_WritePinValue(AMBIENT_LIGHT, HIGH);
			GPT_StartTimer(2000);
	    	door = CLOSE;
	    	caseFlag = 2;
		}

		else if(doorState == UNLOCKED_STATE){
			safe = NOT_SAFE;
			if (door == CLOSE){
				// Third case: if door is closed and the doorState is unlocked ( case 2 is already done )
				// if button pressed during 10 sec then the ambient light will be ON
				if (caseFlag == 0){
					case3();
				}

			}
			// Fifth Case: The initial door state is Unlocked State and the door but a falling edge detected so doorButtonState becomes Locked
			// Ambient Light will be on for 1 sec then off and door will be closed
			else if ((door == OPEN) && (doorButtonState == LOCKED_STATE)){
				door = CLOSE;
				GPIO_WritePinValue(LOCK_LIGHT, LOW);
				GPIO_WritePinValue(HAZARD_LIGHT, LOW);
				GPT_StartTimer(1000);
				caseFlag = 5;
			}
		}


		if(caseFlag == 2){
			case2();
		}
		else if (caseFlag == 5){
			case5();
		}
		else if(caseFlag == 6){
			case6();
		}

		// Fourth case when no buttons pressed for 10 seconds then hazard light should blink 2 times 0.5 second high and 0.5 second low for each blink
		// then lock light and ambient light should be off
	    if(tenSecflag==1 && safe == NOT_SAFE){
			GPIO_WritePinValue(LOCK_LIGHT, LOW);
			case4();
	    }
	}

	}
	return 0;
}


void case2(){
	if (!GPT_CheckTimeIsElapsed())
	{
		timeElapsed = GPT_GetElapsedTime();
		if ((timeElapsed < 500)){
			GPIOB_ODR |= (1 << HAZARD_LIGHT); // Hazard light is on for 0.5 sec
		}
		else if(timeElapsed < 1000){
			GPIOB_ODR &=~ (1 << HAZARD_LIGHT);  // Hazard light is off for 0.5 sec
		}
		else if(timeElapsed < 2000){

		}
	}else{
		GPIO_WritePinValue(AMBIENT_LIGHT, LOW); // when time elapsed Ambient light is off
		caseFlag = 0; // so to know that case 2 is done and door now is Unlocked so we will wait for 10 sec,
					  // if no button pressed hazard light will blink two times
	}
}

void case3(){  // Start 10 sec timer to check if button pressed or not
	if(startTimer == 0){
		GPT_StartTimer(10000);
		startTimer = 1;
	}
	if(!GPT_CheckTimeIsElapsed()){
		if(doorButton){
			GPIO_WritePinValue(AMBIENT_LIGHT, HIGH);
			safe = SAFE;
			door = OPEN;
			startTimer = 0;
		}
		if (handlerPressed){
			// Sixth case: if door is closed and the doorState was unlocked state but the handler is pressed again so the doorState will be locked
			// then hazard light should blink 2 times 0.5 second high and 0.5 second low for each blink then doorState will be locked
			doorState = LOCKED_STATE;
			safe = SAFE;
			caseFlag = 6;
		}
	}
	else{
		if (safe != 1){
			tenSecflag = 1;
			doorState = LOCKED_STATE;
			caseFlag = 3;
			startTimer = 0;
		}

	}
}

void case4(){ // Hazard light will blink two times then ambient light is off
	if(startTimer_4 == 0){
		GPT_StartTimer(2000);
		startTimer_4 = 1;
	}
	if (!GPT_CheckTimeIsElapsed())
	{
		timeElapsed = GPT_GetElapsedTime();
		if ((timeElapsed < 500) || ((timeElapsed > 1000) && (timeElapsed < 1500))){
			GPIOB_ODR |= (1 << HAZARD_LIGHT);
		}
		else{
			GPIOB_ODR &= ~(1 << HAZARD_LIGHT);
		}
	}
	else{
		safe = SAFE;
		tenSecflag = 0;
		GPIO_WritePinValue(AMBIENT_LIGHT, LOW);
    	startTimer_4 = 0;
	}
}

void case5() { // if doorButton locked after it was unlocked then ambient light will be on for 1 sec then off
	if (!GPT_CheckTimeIsElapsed()) {
		GPIO_WritePinValue(AMBIENT_LIGHT, HIGH);
	}
	else{
		GPIO_WritePinValue(AMBIENT_LIGHT, LOW);
		caseFlag = 0;
	}
}


void case6(){ // if door unlocked and it is closed but door button is pressed so door will be locked,
			  // Lock light will be off and hazard led will blink two times
	GPIO_WritePinValue(LOCK_LIGHT, LOW);
	if(startTimer_6 == 0){
		GPT_StartTimer(2000);
		startTimer_6 = 1;
	}
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
		GPIO_WritePinValue(HAZARD_LIGHT, LOW);
		GPIO_WritePinValue(AMBIENT_LIGHT, LOW);
		caseFlag = 0;
		startTimer_6 = 0;
	}
}
