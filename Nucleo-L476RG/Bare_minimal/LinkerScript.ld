MEMORY
{
	FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
	SRAM (xrw) : ORIGIN = 0x20000000, LENGTH = 96K
}

SECTIONS
{
	.text : ALIGN(4)
	{
		*(.isr_vector) /* Vector table */
		*(.text)       /* Program code */
		*(.text*)      /* Merge all .text.* sections inside the .text section */
		KEEP(*(.isr_vector))
	} > FLASH
	
}