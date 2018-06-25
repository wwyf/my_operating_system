#include <semaphore.h>

#define MAX_SEM_NUM 10

semaphore_t semaphore_list[MAX_SEM_NUM];

PUBLIC int sem_solve(uint32_t src_proc, uint32_t sem_type, uint32_t sem_value){
	com_printk("src:%d send msg! sem_type:%d sem_value:%d\n", src_proc, sem_type, sem_value);
	return 2;
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
    
}

/**
 * @brief 唤醒一个进程
 * 
 * 唤醒一个新进程，并且将该进程从信号量的阻塞队列中取出
 * 
 * @param src_pid 
 * @return PRIVATE sem_waitup 
 */
PRIVATE void sem_waitup(int sem_id, int src_pid){

}


PRIVATE int do_get_sem(int sem_value) {
    int use_sem_id = 0;
    /* 默认从1开始遍历，0用作返回错误消息。 */
    for (int i = 1; i < MAX_SEM_NUM; i++){
        /* 一旦找到一个可用的信号量 */
        if (semaphore_list[i].used == 0){
            semaphore_list[i].used == 1;
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

PRIVATE int do_V(int sem_id, int src_pid) {
   semaphore_list[sem_id].value++;
   if (semaphore_list[sem_id].value <= 0){
       sem_waitup(sem_id, src_pid);
   }
}