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
    u16 cluster_code; // 开始簇号
    u32 file_size;
}FileDescriptor;


/* 
初始化文件系统 
将文件描述符写进root数组中 
并且计算各文件使用扇区情况 */
void init_fsystem();


/* tested
从文件名，得到该文件描述符的索引 
FIXME: 如何从返回值得到查找是否成功 */
u16 _fs_find_descriptor_number_by_name(char * file_name);

/* tested
根据文件名寻找文件，11字节的文件名，根据实际文件名大小进行比较。
调用strncmp
返回起始簇号 
0 if not found */
u16 _fs_find_cluster_code_by_name(char * file_name);


/* tested
根据起始簇号获取文件大小 */
u16 _fs_get_file_size_by_cluster_code(u16 cluster_code);


/* tested
打印单个条目的信息 */
void _fs_show_file_by_descriptor_number(u16);

/* tested
根据起始簇号，加载簇到指定地址。
返回值 字节数 */
u16 _fs_load_by_cluster_code(u16 first_cluster_code, void (*program)() );

/* tested
加载文件 到指定地址， 返回程序字节数 */
u16 fs_load_by_name(char * file_name, void (*program)());

/* tested
获取文件占用内存大小 */
u16 fs_get_file_size(char * file_name);

/* tested
打印文件信息表 */
void fs_show_root_file_table();

/* tested
打印单个文件的信息 */
void fs_show_file_by_name(char * file_name);

/* TODO:
打印软盘使用情况 */
void fs_show_floppy_infomation();

#endif