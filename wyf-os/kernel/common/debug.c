#include <type.h>
#include <const.h>
#include <common/common.h>
#include <basic.h>


/*****************************************************************************
 *                           assertion_failure
 *************************************************************************//**
 * Invoked by assert().
 *
 * @param exp       The failure expression itself.
 * @param file      __FILE__
 * @param base_file __BASE_FILE__
 * @param line      __LINE__
 *****************************************************************************/
PUBLIC void assertion_failure(char *exp, char *file, char *base_file, int line)
{
	com_printk("%c  assert(%s) failed: file: %s, base_file: %s, ln%d",
	       MAG_CH_ASSERT,
	       exp, file, base_file, line);

    /* 发生异常后，关闭中断，防止进程切换 */
    _basic_cli();
    while (1){}

	/* should never arrive here */
        __asm__ __volatile__("ud2");
}

PUBLIC void panic(const char *fmt, ...)
{
	int i;
	char buf[256];

	/* 4 is the size of fmt in the stack */
	va_list arg = (va_list)((char*)&fmt + 4);

	i = com_vsprintk(buf, fmt, arg);

	com_printk("%c !!panic!! %s", MAG_CH_PANIC, buf);

	/* should never arrive here */
	__asm__ __volatile__("ud2");
}

