SECTIONS
{
    . = 0x00000;
    .text :
    {
        *(.text)
    }
    . = 0x02000;
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(.bss)
    }
}