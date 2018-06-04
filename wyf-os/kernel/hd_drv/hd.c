/*************************************************************************//**
 *****************************************************************************
 * @file   hd.c
 * @brief  HD driver.
 * @author Forrest Y. Yu
 * @date   2005~2008
 *****************************************************************************
 *****************************************************************************/

#include <type.h>
#include <const.h>
#include <common/debug.h>
#include <basic.h>
#include <hd_drv/hd.h>



PRIVATE void	hd_cmd_out		(hd_cmd_t* cmd);
PRIVATE int	hd_waitfor			(int mask, int val, int timeout);
// PRIVATE void	interrupt_wait		();
PUBLIC	void	hd_identify		(int drive);
PRIVATE void	print_identify_info	(uint16_t* hdinfo);

PRIVATE	uint8_t	hd_status;

PRIVATE	uint8_t	hd_buf[SECTOR_SIZE * 2];


/*****************************************************************************
 *                                init_hd
 *****************************************************************************/
/**
 * Check hard drive, set IRQ handler, enable IRQ and initialize data
 *          structures.
 *****************************************************************************/
PUBLIC void hd_init()
{
	/* Get the number of drives from the BIOS data area */
	// uint8_t * pNrDrives = (uint8_t*)(0x475);
	// printl("NrDrives:%d.\n", *pNrDrives);
	// assert(*pNrDrives);

	// put_irq_handler(AT_WINI_IRQ, hd_handler);
    // TODO:在中断处理函数中的case语句加条14+31的中断向量对应的函数
	_basic_enable_irq(CASCADE_IRQ);
	_basic_enable_irq(AT_WINI_IRQ);
}




/*****************************************************************************
 *                                hd_identify
 *****************************************************************************/
/**
 * Get the disk information.
 * 
 * @param drive  Drive Nr.
 * 暂时只有drive 0 也就是master硬盘驱动器
 *****************************************************************************/
PUBLIC void hd_identify(int drive)
{
	hd_cmd_t cmd;
	cmd.device  = MAKE_DEVICE_REG(0, drive, 0);
	cmd.command = ATA_IDENTIFY;
	hd_cmd_out(&cmd);
	// interrupt_wait();
    

	_basic_port_read(REG_DATA, hd_buf, SECTOR_SIZE);

	print_identify_info((uint16_t*)hd_buf);
}



/*****************************************************************************
 *                                hd_cmd_out
 *****************************************************************************/
/**
 * Output a command to HD controller.
 * 
 * @param cmd  The command struct ptr.
 *****************************************************************************/
PRIVATE void hd_cmd_out(hd_cmd_t * cmd)
{
	/**
	 * For all commands, the host must first check if BSY=1,
	 * and should proceed no further unless and until BSY=0
	 */
	if (!hd_waitfor(STATUS_BSY, 0, HD_TIMEOUT))
		com_print("hd error.");

	/* Activate the Interrupt Enable (nIEN) bit */
	_basic_outb(REG_DEV_CTRL, 0);
	/* Load required parameters in the Command Block Registers */
	_basic_outb(REG_FEATURES, cmd->features);
	_basic_outb(REG_NSECTOR,  cmd->count);
	_basic_outb(REG_LBA_LOW,  cmd->lba_low);
	_basic_outb(REG_LBA_MID,  cmd->lba_mid);
	_basic_outb(REG_LBA_HIGH, cmd->lba_high);
	_basic_outb(REG_DEVICE,   cmd->device);
	/* Write the command code to the Command Register */
	_basic_outb(REG_CMD,     cmd->command);
}

/*****************************************************************************
 *                                waitfor
 *****************************************************************************/
/**
 * Wait for a certain status.
 * 
 * @param mask    Status mask.
 * @param val     Required status.
 * @param timeout Timeout in milliseconds.
 * 
 * @return One if sucess, zero if timeout.
 *****************************************************************************/
PRIVATE int hd_waitfor(int mask, int val, int timeout)
{
	/* TODO:由于没有实现时钟，因此使用循环代替延时 */
	for (int i = 0; i < 1000000; i++){
		if ((_basic_inb(REG_STATUS) & mask) == val)
			return 1;
	}

	return 0;
}


/*****************************************************************************
 *                            print_identify_info
 *****************************************************************************/
/**
 * <Ring 1> Print the hdinfo retrieved via ATA_IDENTIFY command.
 * 
 * @param hdinfo  The buffer read from the disk i/o port.
 *****************************************************************************/
PRIVATE void print_identify_info(uint16_t* hdinfo)
{
	int i, k;
	char s[64];

	struct iden_info_ascii {
		int idx;
		int len;
		char * desc;
	} iinfo[] = {{10, 20, "HD SN"}, /* Serial number in ASCII */
		     {27, 40, "HD Model"} /* Model number in ASCII */ };

	for (k = 0; k < sizeof(iinfo)/sizeof(iinfo[0]); k++) {
		char * p = (char*)&hdinfo[iinfo[k].idx];
		for (i = 0; i < iinfo[k].len/2; i++) {
			s[i*2+1] = *p++;
			s[i*2] = *p++;
		}
		s[i*2] = 0;
		com_printk("%s: %s\n", iinfo[k].desc, s);
	}

	int capabilities = hdinfo[49];
	com_printk("LBA supported: %s\n",
	       (capabilities & 0x0200) ? "Yes" : "No");

	int cmd_set_supported = hdinfo[83];
	com_printk("LBA48 supported: %s\n",
	       (cmd_set_supported & 0x0400) ? "Yes" : "No");

	int sectors = ((int)hdinfo[61] << 16) + hdinfo[60];
	com_printk("HD size: %dMB\n", sectors * 512 / 1000000);
}




/*****************************************************************************
 *                                hd_handler
 *****************************************************************************/
/**
 * Interrupt handler.
 * 
 * @param irq  IRQ nr of the disk interrupt.
 *****************************************************************************/
PUBLIC void irq14_hd_handler()
{
	/*
	 * Interrupts are cleared when the host
	 *   - reads the Status Register,
	 *   - issues a reset, or
	 *   - writes to the Command Register.
	 */
	hd_status = _basic_inb(REG_STATUS);
	com_printk("get hd interrupt!");
	// inform_int(TASK_HD);
}





// /*****************************************************************************
//  *                                task_hd
//  *****************************************************************************/
// /**
//  * Main loop of HD driver.
//  * 
//  *****************************************************************************/
// PUBLIC void task_hd()
// {
// 	MESSAGE msg;

// 	init_hd();

// 	while (1) {
// 		send_recv(RECEIVE, ANY, &msg);

// 		int src = msg.source;

// 		switch (msg.type) {
// 		case DEV_OPEN:
// 			hd_identify(0);
// 			break;

// 		default:
// 			dump_msg("HD driver::unknown msg", &msg);
// 			spin("FS::main_loop (invalid msg.type)");
// 			break;
// 		}

// 		send_recv(SEND, src, &msg);
// 	}
// }


// /*****************************************************************************
//  *                                interrupt_wait
//  *****************************************************************************/
// /**
//  * <Ring 1> Wait until a disk interrupt occurs.
//  * 
//  *****************************************************************************/
// PRIVATE void interrupt_wait()
// {
// 	MESSAGE msg;
// 	send_recv(RECEIVE, INTERRUPT, &msg);
// }
