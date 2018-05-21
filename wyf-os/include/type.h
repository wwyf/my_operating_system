#ifndef _TYPE_H_
#define _TYPE_H_

#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int

#define int8_t char
#define int16_t short
#define int32_t int

#define size_t unsigned int


struct desc_struct_t{
	uint32_t a;
    uint32_t b;
};

typedef struct desc_struct_t desc_table_t[256];

#endif