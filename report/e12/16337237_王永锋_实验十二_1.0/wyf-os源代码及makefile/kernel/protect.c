#include <protect/protect.h>
#include <type.h>
#include <const.h>
#include <global.h>

#define DESCTYPE_LDT 0x82
#define DESCTYPE_TSS 0x89
#define DESCTYPE_TASK 0x85
#define DESCTYPE_INT 0x8e
#define DESCTYPE_TRAP 0x8f
#define DESCTYPE_DPL3 0x8f
/* system */
#define DESCTYPE_s 0x10

void _write_idt_entry(desc_struct_t * dt, 
                    int32_t entry, 
                    uint32_t entry_low, 
                    uint32_t entry_high){
    dt[entry].a = entry_low;
    dt[entry].b = entry_high;
}


void _pack_gate( uint32_t * a, uint32_t * b, 
                uint32_t base, 
                uint8_t seg, 
                uint8_t type, 
                uint8_t flags){
    *a = (seg << 16) | (base & 0xffff);
    *b = (base & 0xffff0000) | ((type & 0xff) << 8) | (flags & 0xff);
}


void _set_gate(uint32_t gate, uint32_t type, void * addr, uint8_t seg){
    uint32_t a,b;
    _pack_gate(&a, &b, (uint32_t)addr, seg, type, 0);
    _write_idt_entry(g_idt_table, gate, a, b);
}

void set_intr_gate(uint32_t n, void * addr){
    _set_gate(n, DESCTYPE_INT, addr, __KERNEL_CS);
}