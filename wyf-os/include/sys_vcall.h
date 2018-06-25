#ifndef _SYS_VCALL_H_
#define _SYS_VCALL_H_

#include <const.h>
#include <type.h>
#include <message.h>

PUBLIC int user_get_ticks();

PUBLIC int user_get_pid();

PUBLIC int user_fork();
PUBLIC int user_wait(int * status);
PUBLIC void user_exit(int status);

PUBLIC int user_get_sem(int sem_value);
PUBLIC int user_free_sem(int sem_id);
PUBLIC int user_sem_p(int sem_id);
PUBLIC int user_sem_v(int sem_id);


#endif // !_SYS_VCALL_H_