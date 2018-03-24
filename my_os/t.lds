SECTIONS
{
    . = 0x10000;
    .text :
    {
        *(.text)
    }
    . = 0x10500;
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(.bss)
    }
}