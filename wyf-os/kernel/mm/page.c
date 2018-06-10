#include <mm/memory.h>
#include <mm/page.h>
#include <global.h>

/* 
这是一个二维数组，第一维指的是可用内存块，第二维的解释如下。
[0] 可用内存
[1] 可用内存物理地址最低地址
[2] 可用内存物理地址最高地址 
*/
physical_address_t available_address[4][3];

void mm_init(){
    /* NOTICE: 可用内存块大小与上面定义的MM_ALLOCATE_SIZE 相同 */
    available_address[0][0] = 0;
    available_address[0][1] = 0x30000;
    available_address[0][2] = 0x40000;
    
    available_address[1][0] = 0;
    available_address[1][1] = 0x40000;
    available_address[1][2] = 0x50000;
    
    available_address[2][0] = 0;
    available_address[2][1] = 0x50000;
    available_address[2][2] = 0x60000;
    
    available_address[3][0] = 0;
    available_address[3][1] = 0x60000;
    available_address[3][2] = 0x70000;
}


/**
 * @brief  给一个进程分配0x10000大小的内存，并获取对应的页表
 * 
 * @param mm_start 需要申请的线性内存起始位置，默认为0x400000
 * @param page_list_ptr 第二级页表。
 * 
 */
void mm_proc_init(page_dir_entry_t * page_dir_ptr, page_list_entry_t * page_list_ptr){
    int allocate_left = MM_ALLOCATE_SIZE;
    /* 在页目录表中的项 */
    int cur_page_dir_index = MM_START >> 22 | 0x3FF;
    /* 在第二级页表中的项 */
    int cur_page_list_index = MM_START >> 12 | 0x3FF;
    /* 线性地址偏移量 */
    int cur_address_offset = MM_START | 0xFFF;
    /* NOTICE:没有做跨页目录表的分配，只对第二级页表进行设置。 */

    /* 寻找可用内存块 */
    int available_memory_block = 0;

    for (int i = 0; i < MM_AVAILABLE_BLOCK_NUM; i++){
        if (available_address[i][0] == 0){
            available_memory_block = i;
            break;
        }
        available_memory_block = -1;   
    }

    int cur_allocate_memory = 0; /** 当前已分配内存的字节大小 */

    /* 设置页表项的内容 */
    cur_allocate_memory = cur_allocate_memory | PG_P  | PG_USU | PG_RWW ;
   

    if (available_memory_block >= 0){
        /* 具有可用内存， 那就进行分配，设置第二级页表 */    
        while (allocate_left > 0){
            page_list_ptr[cur_page_list_index] = cur_allocate_memory;
            cur_page_list_index++;
            cur_allocate_memory += MM_PAGE_SIZE;
            allocate_left -= MM_PAGE_SIZE;
        }

        /* 设置第一级页表 */
        page_dir_ptr[cur_page_dir_index] = (page_dir_entry_t)page_list_ptr;
        return ;
    }
    else {
        /* 没有可用内存 */
        com_printk("ERROR : NO ENOUGH MEMORY!");
        return ;
    }
    return ;
}

/**
 * @brief 切换页目录表中的第一项
 * 
 * 只切换第一项已经足够了
 * 设置线性地址0x4000000及之后的地址对应的页表
 * 
 * @param page_list_ptr 
 */
// void page_switch_to(page_list_entry_t * page_list_ptr){
//     g_page_dir[1] = (page_dir_entry_t)page_list_ptr;
// }

