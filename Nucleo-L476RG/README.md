# Collection of small projects on the Nucleo-L476RG board

## Documents
The directory 'Docs' contains the reference manual and the datasheet for the STM32L476, and the user manual for the Nucleo board.

## Projects
### Bare_minimal
The purpose of the project [Bare_minimal](./Bare_minimal) is to turn on the user LED of the board when the user button is pressed (and turn off when released).  
We do not use the standard start files provided by STM.  
We do not use the HAL API, but instead we set the registers according to the information found in the documents ([Nucleo user manual](./Docs/nucleo64-user_manual.pdf), [reference manual](./Docs/reference_manual.pdf) and [datasheet](./Docs/datasheet.pdf)).  
Also, we write the minimal linker script. 