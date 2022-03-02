# Blinking LED the hard way

In the project we will see the minimal code needed to toggle the user LED on the Nucleo-L476RG board.

## Find where the LED is connected
According to the [Nucleo user manual](../Docs/nucleo64-user_manual.pdf), the user  LED is connected to port A pin 5.

## How to get to port A
First we check the memory mapping from the [datasheet](../Docs/datasheet.pdf) (pages 107 -108).  
We can see that peripherals start at 0x40000000 with GPIOA on bus AHB2 at addresses 0x48000000 - 0x480003FF.  

### GPIO registers
At page 303 of the [reference manual](../Docs/reference_manual.pdf), one can see that GPIOx_MODER is at offset 0x00.
Since the reset value for port A is 0xABFFFFFF and the port configuration I/O pin tells us that "11" means "Analog mode" then after reset PA5 is in analog mode. To set it to output mode we must set bit 10 to 1 and bit 11 to 0. Hence, all we have to do is to xor the current value of the register with 0x800 (this will set bit 11 to 0).

At page 306 of the [reference manual](../Docs/reference_manual.pdf), one can see that GPIOx_ODR is at offset 0x14. Then set/reset of pin 5 is done with bit 5 (OD5).

## Where is RCC
As can be seen page 79 of the [reference manual](../Docs/reference_manual.pdf), RCC is on AHB1 with addresses 0x40021000 - 0x400213FF.

## Clock gating for for GPIOA.
RCC register map is detailled from page 223 to 282 of the [reference manual](../Docs/reference_manual.pdf).  
RCC_AHB2ENR is at offset 0x4C and bit 0 is for GPIOAEN (see page 255 of the [reference manual](../Docs/reference_manual.pdf)).

## Linker script
The linker script is kept to the minimum since all we need is the code section in flash memory and the stack in RAM.

> Note: the linker script is named "LinkerScript.ld" since it is the name expected by STM32CubeIDE. If you rename it then do not forget to change the path in STM32CubeIDE.

## Reset Handler
The reset handler is only performing the led toggling.

## Compiling and flashing the board
For the moment the compilation is done in STM32CubeIDE (*using gcc*) and flashing the board is also done with STM32CubeIDE.

> Note: when creating the project in STM32CubeIDE for the build make sure to not include the standard start files.  
![GCC Config](../Images/gcc_config.png)