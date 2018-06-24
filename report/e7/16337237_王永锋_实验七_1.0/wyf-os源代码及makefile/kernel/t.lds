SECTIONS
{
    . = 0x00000;
    .text :
    {
        *(.text)
    }
    . = 0x15000;
    .data :
    {
        *(.data)
    }
    . = 0x20000;
    .bss :
    {
        *(.bss)
    }
}