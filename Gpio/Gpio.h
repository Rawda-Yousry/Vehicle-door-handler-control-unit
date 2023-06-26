#ifndef GPIO_H
#define GPIO_H
#include "Std_Types.h"
#define GPIOB_BASE_ADDR 0x40020400
#define GPIOB_ODR       (*(uint32 *)(GPIOB_BASE_ADDR + 0x14))
#define GPIOB_MODER     (*(uint32 *)(GPIOB_BASE_ADDR + 0x00))
#define GPIOB_OTYPER    (*(uint32 *)(GPIOB_BASE_ADDR + 0x04))

#define GPIOA_BASE_ADDR 0x40020000
#define GPIOA_MODER  (*(uint32 *)(GPIOA_BASE_ADDR + 0x00))
#define GPIOA_OTYPER (*(uint32 *)(GPIOA_BASE_ADDR + 0x04))
#define GPIOA_ODR    (*(uint32 *)(GPIOA_BASE_ADDR + 0x14))
/*PortName*/
#define GPIO_A 'A'
#define GPIO_B 'B'
#define GPIO_C 'C'
#define GPIO_D 'D'

/*PinMode*/
#define GPIO_INPUT  0x00
#define GPIO_OUTPUT 0x01
#define GPIO_AF     0x02
#define GPIO_ANALOG 0x03

/*DefaultState*/
#define GPIO_PUSH_PULL  0x00
#define GPIO_OPEN_DRAIN 0x01

#define GPIO_NO_PULL_UP_PULL_DOWN 0x00
#define GPIO_PULL_UP              0x01
#define GPIO_PULL_DOWN            0x02
#define GPIO_RESERVED             0x03


/*Data*/
#define LOW  0x00
#define HIGH 0x01

#define OK 1
#define NOK 0


void GPIO_Init(void);
void GPIO_WritePinValue(unsigned char PinId, unsigned char PinData);
unsigned char GPIO_ReadPinState(unsigned char PinId);


#endif /* GPIO_H */
