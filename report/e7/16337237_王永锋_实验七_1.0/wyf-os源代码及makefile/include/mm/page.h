#ifndef _PAGE_H_
#define _PAGE_H_

#include <type.h>

typedef uint32_t page_dir_entry_t;

typedef uint32_t page_list_entry_t;

// 用来给cr3赋值的类型
typedef uint32_t cr3_t;



#define MM_ALLOCATE_SIZE 0x10000
#define MM_START 0x400000
#define MM_AVAILABLE_BLOCK_NUM 4
#define MM_PAGE_SIZE 4096


// ;----------------------------------------------------------------------------
// ; 分页机制使用的常量说明
// ;----------------------------------------------------------------------------

#define PG_P 1 /** 页存在属性位 */
#define PG_RWR 0 /** R/W 属性位值, 读/执行 */
#define PG_RWW 2 /** R/W 属性位值, 读/写/执行 */
#define PG_USS 0 /** U/S 属性位值, 系统级 */
#define PG_USU 4 /** U/S 属性位值, 用户级 */

#endif // !_PAGE_H_