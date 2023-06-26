# Vehicle door handler control unit 
The Vehicle Door Handle Control Unit is a project that implements a system for controlling vehicle door handles with an anti-theft locking capability. The system consists of the following components:

* **Microcontroller Unit:** The core unit responsible for controlling the door handle functions and interacting with the peripherals.

* **Push Buttons:** Two push buttons are used to simulate the handle lock/unlock button and the door lock/unlock button.

* **LEDs:** Three LEDs are used to simulate the vehicle lock, hazard lights, and vehicle ambient light.

The system operates with the following considerations:

* All push buttons are connected as active low.

* All LEDs are connected as active high.


### Use Cases

### Default State
* Initial State: System is powered off.
* Inputs: System is powered on, no button is pressed, the switch is in the on state.
* Resultant System State: Vehicle door is locked. The three LEDs are off.

### Door Unlock
* Initial State: Vehicle door is locked.
* Inputs: Door handle button is pressed.
* Resultant System State: Vehicle door is unlocked but it is closed. Vehicle lock LED is on. Hazard LED blinks once every 0.5 seconds (0.5s high, 0.5s low) for each blink. Ambient light LED is on for 2 seconds and then off.

### Door Open
* Initial State: Vehicle door is unlocked, and the door is closed.
* Inputs: Door unlock button is pressed.
* Resultant System State: Vehicle door is unlocked, and the door is open. Ambient light LED is on.

### Anti-Theft Vehicle Lock
* Initial State: Vehicle door is unlocked, and the door is closed.
* Inputs: No buttons pressed for 10 seconds.
* Resultant System State: Vehicle door is locked. Vehicle lock LED is off. Hazard LED blinks twice every 0.5 seconds (0.5s high, 0.5s low) for each blink. Ambient light LED is off.

### Closing the Door
* Initial State: Vehicle door is unlocked, and the door is open.
* Inputs: Door lock button is pressed.
* Resultant System State: Vehicle door is unlocked, and the door is closed. Vehicle lock LED is off. Hazard LED is off. Ambient light LED is on for one second and then off.

### Locking the Door
* Initial State: Vehicle door is unlocked, and the door is closed.
* Inputs: Door handle lock button is pressed.
* Resultant System State: Vehicle door is locked, and the door is closed. Vehicle lock LED is off. Hazard LED blinks twice every 0.5 seconds (0.5s high, 0.5s low) for each blink. Ambient light LED is off.

## Detailed Deliverables
The following deliverables are required to implement the project:

### GPIO Driver
A GPIO driver is needed to support the connected hardware actions. Implement the following functions:

**void GPIO_Init(void):** Initialize the GPIO registers with the necessary initial values.

**void GPIO_WritePinValue(unsigned char PinId, unsigned char PinData):** Set a value (1 or 0) to a specific pin.

**unsigned char GPIO_ReadPinState(unsigned char PinId):** Return (1) if a falling edge is detected on the pin and (0) otherwise.

The GPIO driver shall be used to read the buttons' state and drive the LEDs' outputs.

### GPT Driver
A GPT driver is needed to handle the timing actions in the project. Implement the following functions:

**void GPT_Init(void):** Initialize the GPT registers with the necessary initial values.

**void GPT_StartTimer(unsigned long int OverFlowTicks):** Request the GPT to start and send its number of ticks before timer overflow and stop.

**unsigned char GPT_CheckTimeIsElapsed(void):** Return (1) if an overflow occurred after the last call of GPT_StartTimer, and (0) if no overflow occurred or GPT_StartTimer was not called since the last read.

**unsigned long int GPT_GetElapsedTime(void):** Return the number of elapsed ticks from the last call of GPT_StartTimer. Return 0 if it has not been called, and 0xffffffff if an overflow occurred.

**unsigned long int GPT_GetRemainingTime(void):** Return the number of remaining ticks until the overflow ticks passed to GPT_StartTimer. Return 0xffffffff if GPT_StartTimer was not called, and 0 if an overflow occurred.

## Simulation Link
https://drive.google.com/file/d/1GqsUYemwj5Vm0Hud0m__u3ptX9gbPbIa/view?usp=drive_link
