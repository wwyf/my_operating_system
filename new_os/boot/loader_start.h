#ifndef _LOADER_READ_H_
#define _LOADER_READ_H_

#include "../include/type.h"
/* 
读取整个扇区到指定内存地址处 */
void read_sector(u16 head, u16 cylinder, u16 sector,  u16 segment, u16 offset);

/* 开启内核 */
void kernel_start();

#endif