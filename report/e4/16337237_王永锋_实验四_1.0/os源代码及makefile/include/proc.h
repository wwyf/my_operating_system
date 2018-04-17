#ifndef _PROC_H_
#define _PROC_H_


/* 初始化进程映像 */
void init_process(int process_index, u16 segment);

/* 调度程序 */
void schedule_process();

#endif