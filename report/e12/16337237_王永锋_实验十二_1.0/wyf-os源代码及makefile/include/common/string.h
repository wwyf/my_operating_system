#ifndef _STRING_H_
#define _STRING_H_
#include <type.h>
/* tested
返回字符串长度 */
size_t com_strlen(char * str);

/* TODO:
复制字符串 */
PUBLIC void com_strcpy(char * dest, char * src);

/* TODO:
复制字符串 */
PUBLIC void com_strncpy(char * dest, char * src, uint32_t size);

/* TODO:
字符串比较 */
PUBLIC int com_strcmp(char * lhs, char * rhs);

/*
字符串比较 */
PUBLIC int com_strncmp(char * lhs, char * rhs, int size);

/* 
使用字符填充一段内存 */
PUBLIC void com_memset(void* p_dst, char ch, size_t size);

/* 
复制一段内存 */
PUBLIC void com_memncpy(void * dest, void * src, uint32_t size);

#endif