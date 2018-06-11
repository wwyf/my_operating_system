#include <global.h>
#include <const.h>
#include <common/common.h>
#include <protect/protect.h>
#include <chr_drv/console.h>
#include <chr_drv/tty_drv.h>
#include <proc/process.h>
#include <hd_drv/hd.h>
#include <message.h>

extern void _proc_restart();
extern void _init_a_process(uint32_t n, char * name, uint32_t pid, void * function, proc_regs_t * k, uint32_t priority);

PRIVATE void _test1();
PRIVATE void _test2();
PRIVATE void _test3();
PRIVATE void _test4();
PRIVATE void _test5();
PRIVATE void _test_get_ticks();
PRIVATE void _test_hd();

void main_test(){
    // _test1();
    // _test2();
    // _test3();
    // _test4();
    // _test5();
    _test_get_ticks();
    // _test_hd();
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
    _init_a_process(1, "test_sys_task", 1, _test_sys_task, (proc_regs_t *)0x20000, 2);
    _init_a_process(3, "test_get_process", 3, _test_get_process, (proc_regs_t *)0x30000, 3);
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
    _init_a_process(0, "test_hd", 0, _test_idle, (proc_regs_t *)0x30000, 1);
    _init_a_process(1, "test_hd", 1, _test_hd_process, (proc_regs_t *)0x20000, 1);
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
    _init_a_process(0, "test1", 1, _test_clock_proc1, (proc_regs_t *)0x20000,1);
    _init_a_process(1, "test2", 2, _test_clock_proc2, (proc_regs_t *)0x30000,1);

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
    _init_a_process(0, "test1", 1, _test_66_proc1, (proc_regs_t *)0x20000,1);
    _init_a_process(1, "test2", 2, _test_66_proc2, (proc_regs_t *)0x30000,1);

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


