#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <type.h>
#include <proc/process.h>

extern uint32_t* interrupt_table;

void interrupt_handler(proc_regs_t * regs);

#endif