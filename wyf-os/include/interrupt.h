#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <type.h>
#include <process.h>

extern uint32_t* interrupt_table;

void interrupt_handler(PT_REGS * regs);

#endif