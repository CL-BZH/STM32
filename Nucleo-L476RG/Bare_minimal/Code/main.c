/*
 * main.c
 *
 *  Created on: March 1st, 2022
 *      Author: Christophe Louargant
 *
 *  Reference Manual:
 *  https://www.st.com/resource/en/reference_manual/rm0351-stm32l47xxx-stm32l48xxx-stm32l49xxx-and-stm32l4axxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 */

#include <stdint.h>

/* memory mapping */
#define FLASH_BASE      0x08000000
#define SRAM_BASE       0x20000000
#define PERIPH_BASE     0x40000000

/* End of RAM address as initial stack pointer */
#define SRAM_SIZE       96*1024 /* STM32L476RG has SRAM1 = 96KB, SRAM2 = 32KB */
#define SRAM_END        (SRAM_BASE + SRAM_SIZE)

/* AHB1 */
#define AHB1_OFFSET     0x20000U
#define AHB1_BASE       (PERIPH_BASE + AHB1_OFFSET)

/* AHB2 */
#define AHB2_OFFSET     0x8000000U
#define AHB2_BASE       (PERIPH_BASE + AHB2_OFFSET)

/* Reset and Clock Control: 0x4002 1000 - 0x4002 13FF */
#define RCC_OFFSET      (0x1000U)
#define RCC_BASE        (AHB1_BASE + RCC_OFFSET)

/* Clock gating
 * for GPIOA -> Set Bit 0 of RCC_AHB2ENR to 1
 * for GPIOC -> Set Bit 2 of RCC_AHB2ENR to 1
 */
#define AHB2ENR_OFFSET  (0x4C)
#define RCC_AHB2ENR     (*(volatile uint32_t*)(RCC_BASE + AHB2ENR_OFFSET))
#define GPIOAEN         (1U<<0)
#define GPIOCEN         (1U<<2)

/* GPIOA peripheral addresses */
#define GPIOA_OFFSET    (0x0U)
#define GPIOA_BASE      (AHB2_BASE + GPIOA_OFFSET)

/* GPIOA mode register */
#define MODER_OFFSET	(0x00U)
#define GPIOA_MODER     (*(volatile uint32_t*)(GPIOA_BASE + MODER_OFFSET))

/* GPIOA output data register */
#define ODR_OFFSET      (0x14U)
#define GPIOA_ODR       (*(volatile uint32_t*)(GPIOA_BASE + ODR_OFFSET))

/* GPIOC peripheral addresses */
#define GPIOC_OFFSET    (0x0800U)
#define GPIOC_BASE      (AHB2_BASE + GPIOC_OFFSET)

/* GPIOC mode register */
#define GPIOC_MODER     (*(volatile uint32_t*)(GPIOC_BASE + MODER_OFFSET))

/* GPIOC input data register */
#define IDR_OFFSET      (0x10U) /* page 306 of the reference manual */
#define GPIOC_IDR       (*(volatile uint32_t*)(GPIOC_BASE + IDR_OFFSET))


void reset_handler(void);

/* vector table */
uint32_t *vector_table[] __attribute__((section(".isr_vector"))) =
{
    (uint32_t *)SRAM_END,     /* initial stack pointer (MSP) */
    (uint32_t *)reset_handler /* Reset_Handler               */
};


void __attribute__((noreturn, weak)) reset_handler() {

	/* Enable clock on GPIOA and GPIOC peripherals */
	RCC_AHB2ENR |= (GPIOAEN | GPIOCEN);

	/* set LED pin as output */
	GPIOA_MODER |= (1U<<10);
	GPIOA_MODER &= ~(1U<<11);

	/* set blue button as input */
	GPIOC_MODER &= ~((1U<<26) | (1U<<27));

	while(1) {
		if(!(GPIOC_IDR & (1U<<13)))
			GPIOA_ODR = (1U<<5);
		else
			GPIOA_ODR &= ~(1U<<5);
	}

}
