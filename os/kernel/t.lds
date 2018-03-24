SECTIONS
{
    . = 0x10000;
    .text :
    {
        *(.text)
    }
    . = 0x12000;
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(.bss)
    }
}