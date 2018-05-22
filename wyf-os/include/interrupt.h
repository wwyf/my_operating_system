#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <type.h>

extern uint32_t interrupt_table;

void interrupt_handler(uint8_t n);

#endif