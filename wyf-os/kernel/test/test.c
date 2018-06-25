#include <global.h>
#include <const.h>
#include <common/common.h>
#include <protect/protect.h>
#include <chr_drv/console.h>
#include <chr_drv/tty_drv.h>
#include <proc/process.h>
#include <hd_drv/hd.h>
#include <message.h>
#include <sys_vcall.h>
#include <mm/mm.h>
#include <semaphore.h>

extern void _proc_restart();
extern void _init_a_process(uint32_t n, char * name, uint32_t pid, void * function, uint32_t priority);

PRIVATE void _test1();
PRIVATE void _test2();
PRIVATE void _test3();
PRIVATE void _test4();
PRIVATE void _test5();
PRIVATE void _test_get_ticks();
PRIVATE void _test_background_task();
PRIVATE void _test_hd();
PRIVATE void _test_fork();
PRIVATE void _test_wait_exit();
PRIVATE void _test_multifork();
PRIVATE void _test_semaphore();

void main_test(){
    // _test1();
    // _test2();
    // _test3();
    // _test4();
    // _test5();
    // _test_get_ticks();
    // _test_background_task();
    // _test_hd();
    // _test_fork();
    _test_wait_exit();
    // _test_multifork();
    // _test_semaphore();
}


/***********************************************/
/* 测试消息机制下的信号量机制
/***********************************************/

int _test_semaphore_id_1;
int _test_semaphore_id_2;
char fruit_disk;
char _test_words[30];


/**
 * @brief 测试使用信号量同步
 * 
 * 必须等到两个子进程，一个说完祝福语，一个放完水果之后，父亲进程才能够回复祝福语。
 * 
 * @return PRIVATE _test_semaphore_1_process 
 */
PRIVATE void _test_semaphore_1_process()
{
   /* 初始化信号量 */
    _test_semaphore_id_1 = user_get_sem(0);
    _test_semaphore_id_2 = user_get_sem(0);

    int pid1 = user_fork();
    
    if (pid1){
        /* 这里会派生出两个子进程，父进程运行下面这一段，子进程运行下面的else */
        com_printk("pid:%d\n", user_get_pid());
        while (1){
            user_sem_p(_test_semaphore_id_1);
            user_sem_p(_test_semaphore_id_2);
            com_printk(_test_words);
            com_memncpy(_test_words,"Father love you, too!", 25);
            com_printk("I eat my fruit %d\n", fruit_disk);
            fruit_disk = 0;
        }
    }
    else{
        /* 这里又派生出两个进程，父进程运行下面的放祝福语，子进程到下面的else放水果*/
        if (user_fork()){
            com_printk("pid:%d\n", user_get_pid());
            while (1){
                com_printk(_test_words);
                com_memncpy(_test_words,"Father, I love you!", 25);
                user_sem_v(_test_semaphore_id_1);
            }
        }
        else{
            com_printk("pid:%d\n", user_get_pid());
            while (1){
                com_printk("The fruit is %d. It's empty!\n", fruit_disk);
                fruit_disk = 5;
                user_sem_v(_test_semaphore_id_2);
            }

        } 
    } 
}


PRIVATE void _test_semaphore(){

    com_memncpy(_test_words,"Father love you, too!", 25);
    _init_a_process(6, "test_semaphore", 6, _test_semaphore_1_process, 1);
    g_cur_proc = &g_pcb_table[6];
    g_cur_proc_context_stack = g_cur_proc->kernel_stack;
    _proc_restart();
}


/* 测试多重fork */
PRIVATE void _test_multifork_process()
{

    int pid1 = user_fork();
    
    if (pid1){
        /* 这里会派生出两个子进程，父进程运行下面这一段，子进程运行下面的else */
        com_printk("pid:%d\n", 6);
        while (1){}
    }
    else{
        int pid2 = user_get_pid();
        com_printk("pid:%d\n", pid2);
        /* 这里又派生出两个进程，父进程运行下面，子进程到下面的else*/
        
        int pid3 = user_fork();
        if (pid3){
            com_printk("pid:%d\n", 8);
            while (1){}
        }
        else{
            com_printk("pid:%d\n", 9);
            while (1){}
        } 
    } 
}

void _delay(){
    for (int i = 0; i < 10000000; i++);
}

PRIVATE void _test_multifork(){
    _init_a_process(6, "test_multifork", 6, _test_multifork_process, 1);
    g_cur_proc = &g_pcb_table[6];
    g_cur_proc_context_stack = g_cur_proc->kernel_stack;
    _proc_restart();
}



/***********************************************/
/* 测试消息机制下的wait 和 exit系统调用
/***********************************************/
PRIVATE void _test_wait_exit_process(){
    com_printk("Init() is running ...\n");
    /* 下面将派生出父进程和子进程 */
	int pid = user_fork();

	if (pid != 0) { /* parent process */
		com_printk("pid(%d) : parent is running, child pid:%d\n", user_get_pid(), pid);
        int status;
        user_wait(&status);
        com_printk("pid(%d) : child has exited, exit status is %d\n", user_get_pid(), status);
	}
    
	else {	/* child process */
		com_printk("pid(%d) : child is running, pid:%d, ret:%d\n", user_get_pid(), user_get_pid(), pid);
        user_exit(10);
	}
    while (1){}
}

PRIVATE void _test_wait_exit(){
    _init_a_process(6, "test_wait_exit", 6, _test_wait_exit_process, 3);
    g_cur_proc = &g_pcb_table[6];
    g_cur_proc_context_stack = g_cur_proc->kernel_stack;
    _proc_restart();
}




/***********************************************/
/* 测试消息机制下的fork系统调用
/* NOTICE:在schedule.c中设置当前进程数量
/***********************************************/

PRIVATE void _test_fork_process(){
	com_printk("Init() is running ...\n");
	int pid = user_fork();
	if (pid != 0) { /* parent process */
		com_printk("parent is running, child pid:%d\n", pid);
	}
	else {	/* child process */
		com_printk("child is running, pid:%d\n", user_get_pid());
	}
    while (1){}
}


PRIVATE void _test_fork(){
    _init_a_process(6, "test_fork_process", 6, _test_fork_process, 3);
    g_cur_proc = &g_pcb_table[6];
    g_cur_proc_context_stack = g_cur_proc->kernel_stack;
    _proc_restart();
}




/***********************************************/
/* 测试消息机制下的get_ticks and get_pid系统调用
/* NOTICE:在schedule.c中设置当前进程数量
/***********************************************/


PRIVATE void _test_process_get_process(){
    while(1){
        for (int i = 0; i <100000; i++){
            for (int j = 0; j < 1000; j++);
        }
        com_printk("<Ticks:%d>\n", user_get_ticks());
        com_printk("<Pid:%d>\n", user_get_pid());
    }
}

PRIVATE void _test_process_get_process_2(){
    while(1){
        for (int i = 0; i <100000; i++){
            for (int j = 0; j < 1000; j++);
        }
        com_printk("<Ticks:%d>\n", user_get_ticks());
        com_printk("<Pid:%d>\n", user_get_pid());
    }
}



PRIVATE void _test_background_task(){
    _init_a_process(5, "test_get_process", 5, _test_process_get_process, 3);
    _init_a_process(6, "test_get_process", 6, _test_process_get_process, 3);
    g_cur_proc = &g_pcb_table[5];
    g_cur_proc_context_stack = g_cur_proc->kernel_stack;
    // _basic_cli();
    _proc_restart();
}


/***********************************************/
/* 测试消息机制下的get_ticks系统调用
/* NOTICE:在schedule.c中设置当前进程数量
/***********************************************/

PRIVATE int _test_sys_call_get_ticks(){
	message_t msg;
	msg_reset(&msg);
	msg.type = GET_TICKS;
    /* NOTICE: TASK_SYS 指的是ssy_task的进程号 */
	msg_send_recv(BOTH, TASK_SYS, &msg);
	// msg.RETVAL = g_ticks;
	return msg.RETVAL;
}

PRIVATE void _test_sys_task(){
	message_t msg;
	while (1) {
        for (int i = 0; i < 100000000; i++);
        com_printk("in the sys task");
		msg_send_recv(RECEIVE, ANY, &msg);
		int src = msg.source;

		switch (msg.type) {
		case GET_TICKS:
			msg.RETVAL = g_ticks;
			msg_send_recv(SEND, src, &msg);
			break;
		default:{}
			panic("unknown msg type");
			break;
		}
	}
}

PRIVATE void _test_get_process(){
    while(1){
        for (int i = 0; i <100000; i++){
            for (int j = 0; j < 1000; j++);
        }
        com_printk("<Ticks:%d>", _test_sys_call_get_ticks());
    }
}

PRIVATE void _test_get_ticks(){
    _init_a_process(1, "test_sys_task", 1, _test_sys_task, 2);
    _init_a_process(3, "test_get_process", 3, _test_get_process,  3);
    g_cur_proc = &g_pcb_table[1];
    g_cur_proc_context_stack = g_cur_proc->kernel_stack;
    // _basic_cli();
    _proc_restart();
}


/***********************************************/
/* 测试硬盘驱动
/* NOTICE:在schedule.c中设置当前进程数量
/***********************************************/
void _test_idle(){
    while (1){
        for (int i = 0; i <10000000; i++);
        com_printk("pid : %d", g_cur_proc->pid);
    }
}

void _test_hd_process(){
    while (1){
        for (int i = 0; i <10000000; i++);
        com_printk("pid : %d", g_cur_proc->pid);
        // hd_identify(0);
    }
}


void _test_hd(){
    _init_a_process(0, "test_hd", 0, _test_idle,1);
    _init_a_process(1, "test_hd", 1, _test_hd_process, 1);
    g_cur_proc = &g_pcb_table[0];
    g_cur_proc_context_stack = g_cur_proc->kernel_stack;
    _proc_restart();
}

/***********************************************/
/* 测试内核进程的初始化和切换 */
/* 使用时钟中断中断作为进程切换的测试中断 */
/***********************************************/

void _test_clock_proc1(){
    while (1){
        for (int i = 0; i < 10000000; i++);
        com_print("pid : 1");
    }
}

void _test_clock_proc2(){
    while (1){
        for (int i = 0; i < 10000000; i++);
        com_printk("pid : 2");
    }
}


void _test5(){
    _init_a_process(0, "test1", 1, _test_clock_proc1,1);
    _init_a_process(1, "test2", 2, _test_clock_proc2,1);

    g_cur_proc = &g_pcb_table[0];
    g_cur_proc_context_stack = g_cur_proc->kernel_stack;
    _proc_restart();
}




/***********************************************/
/* 测试内核进程的初始化和切换 */
/* 使用0x66中断作为进程切换的测试中断，可在process.c中修改中断号 */
/***********************************************/

void _test_66_proc1(){
    while (1){
        for (int i = 0; i <100000000; i++);
        com_print("pid : 1");
        asm("int $0x66");
    }
}

void _test_66_proc2(){
    while (1){
        for (int i = 0; i <100000000; i++);
        com_printk("pid : 2");
        asm("int $0x66");
    }
}


void _test4(){
    _init_a_process(0, "test1", 1, _test_66_proc1,1);
    _init_a_process(1, "test2", 2, _test_66_proc2, 1);

    g_cur_proc = &g_pcb_table[1];
    g_cur_proc_context_stack = g_cur_proc->kernel_stack;
    _basic_sti();
    _proc_restart();
}



/***********************************************/
/* 测试中断能够做到正确的保存环境和恢复环境。 */
/***********************************************/

void _test3(){
    while(1){
        asm("int $40");
        asm("int $48");
        // while(1){}
    }
}

/***********************************************/
/* 用于测试自定义中断的设置 */

void example_interrupt(){
    com_print("test in interrupt!!");
    com_print("test success!!!!!!");
}

void _test2(){
    com_print("test default interrupt!");
    com_print("testing interrupt!");
    set_intr_gate(0x79, example_interrupt);
    asm("int $0x79");
    com_print("return to kernel");
    while(1){}
}


/***********************************************/

// /* 用来测试字符设备的使用 */
void _test1(){
    _console_set_cursor(0);
    _console_set_cursor(80);
    com_print("asdfasf!");
    com_printk("com_printk!!test!!com_printk%d", 11122);
    _console_set_cursor(1920);
    _tty_queue_put(&((&g_tty_table[0])->write_q), 'a');
    com_print("head: %d" ,g_tty_table[0].write_q.head);
    com_print("tail: %d" ,g_tty_table[0].write_q.tail);
    com_print("data: %c" ,g_tty_table[0].write_q.buf[g_tty_table[0].write_q.head]);
    _console_write(&g_tty_table[0]);
    _tty_queue_put(&(&g_tty_table[0])->write_q, 'x');
    _tty_queue_put(&(&g_tty_table[0])->write_q, 'a');
    _tty_queue_put(&(&g_tty_table[0])->write_q, 'c');
    _tty_queue_put(&(&g_tty_table[0])->write_q, 'd');
    _tty_queue_put(&(&g_tty_table[0])->write_q, 'e');
    _tty_queue_put(&(&g_tty_table[0])->write_q, 'b');
    _tty_queue_put(&(&g_tty_table[0])->write_q, 'i');
    _console_write(&g_tty_table[0]);
    char t[16] = "!!testtesttest";
    tty_write(0, t, com_strlen(t));
    __asm__("int $0x80");
    char s[30];
    com_sprintk(s, "test %d%s", 2342,"sdfs");
    com_printk(s);
    // while(1){};
}


