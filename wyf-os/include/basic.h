#ifndef _BASIC_H_
#define _BASIC_H_

#include <type.h>



/* tested
将8位的数据写到指定端口 */
void _basic_outb(uint16_t port_number, uint8_t data);

/* tested
从指定端口中读出8位数据，并作为返回值返回 */
uint8_t _basic_inb(uint16_t port_number);



#endif