#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <mm/page.h>

typedef struct{
    page_dir_entry_t * mm_page_dir;
    page_list_entry_t * mm_page_list; /** 进程对应的页表 */
}mm_struct_t;

/* 内核进程专用 */
extern mm_struct_t init_kernel_mm;

/* 线性地址 */
typedef uint32_t linear_address_t;

/* 物理地址 */
typedef uint32_t physical_address_t;

/* 衡量内存大小的单位：字节数量 */

typedef uint32_t mm_size_t;

// void free_page();

// void copy_page_tables();

// void get_empty_tables();

// void free_page_tables();

// void put_page();

// void do_wp_page();

// void do_no_page();

// void do_page_fault();

#endif // !_MEMORY_H_