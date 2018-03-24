#ifndef _BASIC_H_
#define _BASIC_H_

#include "type.h"

void write_memory_word(u32 address, u16 data);
void write_memory_byte(u32 address, u8 data);
void write_port_byte(u16 port_number, u8 data);
u8 read_port_byte(u16 port_number);
u16 read_memory_word(u32 address);

#endif