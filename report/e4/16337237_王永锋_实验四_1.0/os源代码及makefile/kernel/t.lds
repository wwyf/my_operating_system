SECTIONS
{
    . = 0x00000;
    .text :
    {
        *(.text)
    }
    . = 0x04000;
    .data :
    {
        *(.data)
    }
    . = 0x05000;
    .bss :
    {
        *(.bss)
    }
}