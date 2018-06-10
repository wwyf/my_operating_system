#include <type.h>
#include <const.h>
#include <common/common.h>
/**
 * @brief 清空消息，使用0填充消息
 * 
 * @param p 
 * @return PUBLIC reset_msg 
 */
PUBLIC void msg_reset(message_t* p)
{
	com_memset(p, 0, sizeof(message_t));
}
