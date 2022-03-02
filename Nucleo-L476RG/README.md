# Collection of small projects on the Nucleo-L476RG board

## Documents
The directory 'Docs' contains the reference manual and the datasheet for the STM32L476, and the user manual for the Nucleo board.

## Projects
### Bare_minimal
The purpose of the project [Bare_minimal](./Bare_minimal) is to toggle the user LED of the board by setting the registers (RCC, GPIO, ...) by hand.  
We will not use the standard start files provided by STM and write the minimal linker script. 