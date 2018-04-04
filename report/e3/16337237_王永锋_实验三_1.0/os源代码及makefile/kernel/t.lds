SECTIONS
{
    . = 0x00000;
    .text :
    {
        *(.text)
    }
    . = 0x02100;
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(.bss)
    }
}