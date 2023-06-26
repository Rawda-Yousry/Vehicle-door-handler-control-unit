#ifndef GPIO_H
#define GPIO_H
#include "Std_Types.h"
#include "Gpio_Private.h"

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


void GPIO_Init(void);
void GPIO_WritePinValue(unsigned char PinId, unsigned char PinData);
unsigned char GPIO_ReadPinState(unsigned char PinId);


#endif /* GPIO_H */
