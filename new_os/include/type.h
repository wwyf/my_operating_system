
#ifndef	_ORANGES_TYPE_H_
#define	_ORANGES_TYPE_H_

typedef unsigned int size_t;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef int s32;
typedef short s16;
typedef char s8;
typedef char ** va_list ;

#define INT_LENGTH 15
#define BUF_LENGTH 1024

#define MAX_SYSTEM_CALL 256

#define MSG_SEGMENT 0x0100
#define MSG_SEND 0x0000
#define MSG_RECV 0x5000



typedef struct{
    u8 opcode;
    u16 number;
    u32 space1;
    u32 space2;
    u32 space3;
    u32 space4;
    u32 space5;
}Message;



typedef struct proc_register {
    u16 sp;// 用户栈指针
    u16 ss;// 用户所处栈段
    // #####
    u16 es;
    u16 ds;
    // popad
    u32 edi;
    u32 esi;
    u32 ebp;
    u32 placeholder; 
    u32 ebx;
    u32 edx;
    u32 ecx;
    u32 eax;
    // pushad
    // u32 retaddr;
    u16 ip;
    u16 cs;
    u16 flags;
    // #####
}procRegister;


typedef struct controlProcessBlock{
    procRegister regs;
    u32 pid;
    char p_name[16];
}PCB_t;


#endif