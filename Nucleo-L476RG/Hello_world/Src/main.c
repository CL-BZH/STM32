/**
 ******************************************************************************
 * 
 ******************************************************************************
 */


typedef unsigned char uint8_t;
typedef long unsigned int uint32_t;

/* 
 * Debug Exception and Monitor Control Register base address
 * https://developer.arm.com/documentation/ddi0337/e/CEGHJDCF
 */
#define CoreDebug_BASE  (0xE000EDF0UL)   /* Core Debug Base Address */
#define DEMCR_OFFSET    (0x00CUL)        /* Debug Exception and Monitor Control Register */
#define DEMCR        	*((volatile uint32_t*)(CoreDebug_BASE + DEMCR_OFFSET))

/*
 * ITM register addresses
 * page 713 of armv7 manual
 */
#define ITM_STIM0       *((volatile uint32_t*)0xE0000000UL)
#define ITM_TER0        *((volatile uint32_t*)0xE0000E00UL)

/* Messages to print */
const char * str[] = {"Hello world\r", "Bye world  \r"};

static inline void ITM_SendChar(uint8_t ch) {

	/* Enable TRCENA */
	DEMCR |= (1UL << 24); /* page 706 of armv7 manual */

	/* enable stimulus port 0 */
	ITM_TER0 |= ( 1 << 0);

	/* FIFO status: bit 0 */
	while(!(ITM_STIM0 & 1))
	  __asm("nop");

	/* Write to ITM stimulus port0 */
	ITM_STIM0 = ch;
}

/*
 * Very basic function to print a C string
 */
static inline void debug_print(const char * cstring) {
	const char * ch_ptr = cstring;
	while(*ch_ptr != '\0')
		ITM_SendChar(*ch_ptr++);
}

static inline void busy_wait(uint32_t count) {
    while(count--)
      __asm("nop");
}

int main(void) {
  uint8_t idx = 0;
  
  while(1) {
    debug_print(str[idx++ %2]);
    busy_wait(400000);
  }
}
