
/* Linker script for nRF52 DFU */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x26000, LENGTH = 0x56000  /* Application region */
  UICR (r)   : ORIGIN = 0x10001000, LENGTH = 0x100
}

SECTIONS
{
  .text : {
    KEEP(*(.vectors))
    *(.text*)
    *(.rodata*)
    *(.init)
    *(.fini)
    KEEP(*(.init_array*))
    KEEP(*(.fini_array*))
  } > FLASH

  .data : {
    *(.data*)
  } > FLASH

  .bss : {
    *(.bss*)
    *(COMMON)
  } > FLASH
}
