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

/* Clock gating for GPIOA -> Set Bit 0 of RCC_AHB2ENR to 1 */
#define AHB2ENR_OFFSET  (0x4C)
#define RCC_AHB2ENR     (*(volatile uint32_t*)(RCC_BASE + AHB2ENR_OFFSET))
#define GPIOAEN         (1U)

/* GPIOA peripheral addresses */
#define GPIOA_OFFSET    (0x0U)
#define GPIOA_BASE      (AHB2_BASE + GPIOA_OFFSET)

/* Set GPIO mode register */
#define MODER_OFFSET	(0x00U)
#define GPIOA_MODER     (*(volatile uint32_t*)(GPIOA_BASE + MODER_OFFSET))

/* Set GPIO output data register */
#define ODR_OFFSET      (0x14U)
#define GPIOA_ODR       (*(volatile uint32_t*)(GPIOA_BASE + ODR_OFFSET))

#define DELAY           400000


static inline void gpioa_clock_enable() {
	RCC_AHB2ENR |= GPIOAEN;
}

static inline void set_gpioa_pin5_to_output_mode() {
	GPIOA_MODER |= (1U<<10);
	GPIOA_MODER &= ~(1U<<11); /* Same as GPIOA_MODER ^= 0x800 */
}

static inline void toggle_led() {
	/* LED connected to PIN 5 of GPIOA */
	GPIOA_ODR ^= (1U<<5);
}

static inline void busy_wait(uint32_t count) {
    while(count--)
    	asm volatile("");
}

void reset_handler(void);

/* vector table */
uint32_t *vector_table[] __attribute__((section(".isr_vector"))) =
{
    (uint32_t *)SRAM_END,     /* initial stack pointer (MSP) */
    (uint32_t *)reset_handler /* Reset_Handler               */
};


void __attribute__((noreturn, weak)) reset_handler() {

	/* Clock gating: enable clock on GPIOA peripheral */
	gpioa_clock_enable();

	/* set LED pin as output */
	set_gpioa_pin5_to_output_mode();

	while(1) {
		toggle_led();

		busy_wait(DELAY);
	}

}
