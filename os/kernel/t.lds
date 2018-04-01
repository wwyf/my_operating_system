SECTIONS
{
    . = 0x00000;
    .text :
    {
        *(.text)
    }
    . = 0x03000;
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(.bss)
    }
}