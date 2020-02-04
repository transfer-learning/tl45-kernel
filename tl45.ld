SECTIONS
{
  . = 0x0;
  .raw : { *(.text .data .rodata) }
}