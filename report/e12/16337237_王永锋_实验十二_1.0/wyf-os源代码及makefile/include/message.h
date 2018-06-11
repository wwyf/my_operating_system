#ifndef _MESSAFE_H_
#define _MESSAFE_H_

#include <type.h>

PUBLIC void msg_reset(message_t* p);

PUBLIC int msg_send_recv(int function, int src_dest, message_t* msg);

/* 定义在syscall中 */
extern int sendrec(int function, int src_dest, message_t * msg);


#endif // !_MESSAFE_H_