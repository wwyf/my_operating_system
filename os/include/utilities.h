__asm__(
    "mov    %cs,%ax\n\t"
    "mov    %ax,%ds\n\t"
    "mov    %ax,%es\n\t"
    "mov    %ax,%ss\n\t"
    "mov    $0x5000,%sp\n\t"
    "jmp   main\n\t"
);