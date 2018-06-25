#ifndef _TYPE_H_
#define _TYPE_H_

#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define uint64_t unsigned long long

#define int8_t char
#define int16_t short
#define int32_t int
#define int64_t long long

#define size_t unsigned int

/* 函数类型 */
#define	PUBLIC		/* PUBLIC is the opposite of PRIVATE */
#define	PRIVATE	static	/* PRIVATE x limits the scope of x */


typedef struct desc_struct{
	uint32_t a;
    uint32_t b;
}desc_struct_t;


/**
 * @brief 进程间通信的格式
 * 
 * MESSAGE mechanism is borrowed from MINIX
 */
struct mess1 {
	int m1i1;
	int m1i2;
	int m1i3;
	int m1i4;
};
struct mess2 {
	void* m2p1;
	void* m2p2;
	void* m2p3;
	void* m2p4;
};
struct mess3 {
	int	m3i1;
	int	m3i2;
	int	m3i3;
	int	m3i4;
	uint64_t m3l1;
	uint64_t m3l2;
	void*	m3p1;
	void*	m3p2;
};
typedef struct {
	int source;
	int type;
	union {
		struct mess1 m1;
		struct mess2 m2;
		struct mess3 m3;
	}u;
}message_t;



typedef desc_struct_t desc_table_t[256];

#endif