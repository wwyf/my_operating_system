#include <semaphore.h>
#include <global.h>
#include <common/common.h>

#define MAX_SEM_NUM 20

semaphore_t semaphore_list[MAX_SEM_NUM];

PRIVATE int do_V(int sem_id);
PRIVATE int do_P(int sem_id, int src_pid);
PRIVATE int do_free_sem(int sem_id);
PRIVATE int do_get_sem(int sem_value);

/**
 * @brief 
 * 
 * @param src_proc 
 * @param sem_type 
 * @param sem_value 对于get_sem方法而言是信号量的value，对于其他方法而言是sem_id
 * @return PUBLIC sem_solve 
 */
PUBLIC int sem_solve(uint32_t src_proc, uint32_t sem_type, uint32_t sem_value){
	com_printk("src:%d send msg! sem_type:%d sem_value:%d\n", src_proc, sem_type, sem_value);
    int ret;
    switch (sem_type){
        case SEM_GETSEM:
            ret = do_get_sem(sem_value);
            return ret;
            break;
        case SEM_FREESEM:
            ret = do_free_sem(sem_value);
            return ret;
            break;
        case SEM_P:
            ret = do_P(sem_value, src_proc);
            return ret;
            break;
        case SEM_V:
            ret = do_V(sem_value);
            return ret;
            break;
    }
	return 99;
}


/**
 * @brief 阻塞该进程，并且放到
 * 
 * 阻塞该进程，并且将该进程加入到信号量的阻塞队列中。
 * 
 * @param src_pid 
 * @return PRIVATE sem_block 
 */
PRIVATE void sem_block(int sem_id, int src_pid){
    /* 将源进程状态调整为sleep */
    proc_block_pid(src_pid);

    /* 将源进程放到信号量的阻塞队列中 */
    proc_task_struct_t * q = semaphore_list[sem_id].next;
    /* 寻找一个用于放置信号量阻塞进程的pcb */
    if (q == NULL){
        /* 如果当前信号量一个进程都没有阻塞 */
        semaphore_list[sem_id].next = &g_pcb_table[src_pid];
    }
    else {
        /* 当前信号量已经阻塞了一些进程 */
        proc_task_struct_t * next_q = q->sem_next;
        /* q 当前PCB， next_q，下一个PCB
        循环直到next_q等于NULL */
        while (next_q != NULL){
            q = next_q;
            next_q = next_q->sem_next;
        }
        /* 此时找到了一个具有空sem_next的进程PCB */
        next_q->sem_next = &g_pcb_table[src_pid];
    }
}

/**
 * @brief 唤醒一个进程
 * 
 * 从信号量中的阻塞队列，唤醒队头
 * 并且将该进程从信号量的阻塞队列中取出
 * 
 * @param src_pid 
 * @return PRIVATE sem_waitup 
 */
PRIVATE void sem_waitup(int sem_id){
    /* 从阻塞队列中，找到队头*/
    proc_task_struct_t * head = semaphore_list[sem_id].next;

    /* NOTICE:队头是否为空 */

    /* 唤醒队头 */
    int wake_pid = head->pid;
    proc_wake_pid(wake_pid);

    /* 将队头从阻塞队列中清除 */
    proc_task_struct_t * next_head = head->sem_next;
    head->sem_next = NULL;
    semaphore_list[sem_id].next = next_head;
}


PRIVATE int do_get_sem(int sem_value) {
    int use_sem_id = 0;
    /* 默认从1开始遍历，0用作返回错误消息。 */
    for (int i = 1; i < MAX_SEM_NUM; i++){
        /* 一旦找到一个可用的信号量 */
        if (semaphore_list[i].used == 0){
            semaphore_list[i].used = 1;
            semaphore_list[i].value = sem_value;
            return i;
        }
    }
    /* 找不到可用的信号量，返回0 */
    return 0;
}

PRIVATE int do_free_sem(int sem_id) {
    semaphore_list[sem_id].used = 0;
}


PRIVATE int do_P(int sem_id, int src_pid) {
   semaphore_list[sem_id].value--;
   if (semaphore_list[sem_id].value < 0){
       sem_block(sem_id, src_pid);
   }
}

PRIVATE int do_V(int sem_id) {
   semaphore_list[sem_id].value++;
   if (semaphore_list[sem_id].value <= 0){
       sem_waitup(sem_id);
   }
}