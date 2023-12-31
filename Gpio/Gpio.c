#include "Gpio.h"

#include "Gpio_Private.h"
#include "Utils.h"

unsigned char state = 0;
unsigned char last_state = 1;

void GPIO_Init(void){
  uint8 i = 0;
  // Push buttons
  for(i = 0; i <= 1; i++){
    GPIOB_MODER &= ~(0x03 << (i * 2));
    GPIOB_MODER |= (GPIO_INPUT << (i * 2));

    GPIOB_PUPDR &= ~(0x03 << (i * 2));
    GPIOB_PUPDR |= (GPIO_PULL_UP << (i * 2));
  }

  // LEDs
  for(i = 2; i <= 4; i++){
    GPIOB_MODER &= ~(0x03 << (i * 2));
    GPIOB_MODER |= (GPIO_OUTPUT << (i * 2));

    GPIOB_OTYPER &= ~(0x01 << i);
    GPIOB_OTYPER |= (GPIO_PUSH_PULL << i);
  }
}


void GPIO_WritePinValue(unsigned char PinId, unsigned char PinData){
  if(PinData == 1){
    GPIOB_ODR &= ~(0x01 << PinId);
    GPIOB_ODR |= (PinData << PinId);
  }
  else{
    GPIOB_ODR &= ~(0x01 << PinId);
  }
  
}


unsigned char GPIO_ReadPinState(unsigned char PinId){
  state = (GPIOB_IDR & (1 << PinId)) >> PinId;

  if(state == 0 && last_state == 1){
    last_state = state;
    return 1;
  }
  else{
    last_state = state;
    return 0;
  }
}

