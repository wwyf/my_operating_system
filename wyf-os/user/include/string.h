#ifndef _STRING_H_
#define _STRING_H_
#include <type.h>
/* tested
返回字符串长度 */
size_t strlen(char * str);

/* TODO:
复制字符串 */
void strcpy(char * dest, char * src);

/* TODO:
复制字符串 */
void strncpy(char * dest, char * src, uint32_t size);

/* TODO:
字符串比较 */
int strcmp(char * lhs, char * rhs);

/*
字符串比较 */
int strncmp(char * lhs, char * rhs, int size);

/* TODO:
字符串拼接*/
void strncat(char * dest, char * src, uint32_t size);


/* TODO:
finds the first occurrence of a character */
char * strchr(char *str, int ch );

/* TODO:
Finds the first occurrence of the null-terminated byte string pointed to by substr in the null-terminated byte string pointed to by str. */
char *strstr(char* str,char* substr );


/* TODO:
searches an array for the first occurrence of a character  */
void* memchr( const void* ptr, int ch, size_t count );
/* TODO:
compares two buffers */
int memcmp( const void* lhs, const void* rhs, size_t count );
 
/* TODO:
fills a buffer with a character */
void *memset( void *dest, int ch, size_t count );

/* TODO:
copies one buffer to another */
void* memcpy( void *dest, const void *src, size_t count );
 
/* TODO:
moves one buffer to another  */
void* memmove( void* dest, const void* src, size_t count );

#endif