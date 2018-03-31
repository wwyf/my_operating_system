#ifndef _FSYSTEM_H_
#define _FSYSTEM_H_

typedef struct{
    char file_name1;
    char file_name2;
    char file_name3;
    char file_name4;
    char file_name5;
    char file_name6;
    char file_name7;
    char file_name8;
    char file_name9;
    char file_name10;
    char file_name11;
    u8 file_attributes;
    u32 useless1;
    u32 useless2;
    u16 useless3;
    u16 last_write_time;
    u16 last_write_data;
    u16 cluster_number; // 开始簇号
    u32 file_size;
}FileDescriptor;



#endif