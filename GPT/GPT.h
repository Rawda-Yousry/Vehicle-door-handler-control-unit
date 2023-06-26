#ifndef GPT_H_
#define GPT_H_

//#include "Gpio.h"
#include "Std_Types.h"
#define NVIC_ISER0 (*(uint32 *)0xE000E100)
typedef struct{
    uint32 CR1;
    uint32 CR2;
    uint32 SMCR;
    uint32 DIER;
    uint32 SR;
    uint32 EGR;
    uint32 CCMR1;
    uint32 CCMR2;
    uint32 CCER;
    uint32 CNT;
    uint32 PSC;
    uint32 ARR;
    uint32 CCR1;
    uint32 CCR2;
    uint32 CCR3;
    uint32 CCR4;
    uint32 DCR;
    uint32 DMAR;
    uint32 OR;
} TIMType;

#define TIM2 ((TIMType *) 0x40000000) 
#define CR1_CEN 0
#define CR1_URS 2
#define CR1_OPM 3
#define CR1_DIR 4
#define CR1_CMS 5  //5,6
#define CR1_ARPE 7

void GPT_Init(void);
void GPT_StartTimer(uint32 OverFlowTicks);
unsigned char GPT_CheckTimeIsElapsed(void);
uint32 GPT_GetElapsedTime(void);
uint32 GPT_GetRemainingTime(void);




#endif /* GPT_H_ */
