#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

/*
用于修改中断向量表，安装新的中断。
*/

/*
触发中断 */
void trigger_interrupt_int40();


/*
中断返回 */
void return_from_interrupt();


#endif