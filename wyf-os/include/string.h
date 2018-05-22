#ifndef _STRING_H_
#define _STRING_H_
#include <type.h>
/* tested
返回字符串长度 */
size_t strlenk(char * str);

/* TODO:
复制字符串 */
void strcpyk(char * dest, char * src);

/* TODO:
复制字符串 */
void strncpy(char * dest, char * src, uint32_t size);

/* TODO:
字符串比较 */
int strcmpk(char * lhs, char * rhs);

/*
字符串比较 */
int strncmpk(char * lhs, char * rhs, int size);

#endif