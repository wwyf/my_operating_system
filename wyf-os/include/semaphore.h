#ifndef _SEMAPHORE_H_

#include <type.h>
#include <const.h>
#include <proc/process.h>



typedef struct {
	int value;
	proc_task_struct_t * next;
	int used;
}semaphore_t;


PUBLIC int sem_solve(uint32_t src_proc, uint32_t sem_type, uint32_t sem_value);

#endif // !_SEMAPHORE_H_