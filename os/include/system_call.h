#ifndef _SYSTEM_CALL_H_
#define _SYSTEM_CALL_H_


/*
用于修改自定义中断向量表，安装新的中断，并调用int 40。
*/
void install_system_call(int system_call_number, void (*function)() );

#endif