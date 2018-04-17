/* 默认内核为root表项中的第一个文件，这样子就不需要做字符串比较的工作，就可以减少代码的依赖 */
/* TODO:加载器所在段，0x0800 需要常量 */
/* 加载器所在位置：0x8000-0x0a000 */
#include "../include/utilities.h"
#include "./loader_start.h"
#include "../include/type.h"

#define LOADER_SEGMENT 0x0800
#define ROOT_SECTOR_NUMBER 37

u16 _fs_load_by_cluster_code(u16 cluster_code, u16 segment, u16 offset );
void read_n_sector(u16 sector_code, u16 number,  u16 segment, u16 offset);

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


u16 FAT_table[256]; // 0x420
FileDescriptor root[32]; // 0x620
u16 root_sector;  // 3e0
u16 first_cluster_by_sector; // 0xa02
u16 cluster2sector; // 0xa00
u16 kernel_cluster_code; // 0x3e2
int main(){
    cluster2sector = 1;
    // TODO:可能需要换成常量
    root_sector = ROOT_SECTOR_NUMBER;
    first_cluster_by_sector = root_sector + 1;
    // 读取root文件表项
    read_n_sector(1,1,LOADER_SEGMENT,(u32)&FAT_table);
    // TODO:系统内核是fat表中的第一项
    read_n_sector(root_sector,1, LOADER_SEGMENT,(u32)&root);
    // TODO: 系统内核的簇号一个扇区应该放得下
    int kernel_index = 0;
    kernel_cluster_code = root[0].cluster_code;
    // kernel_cluster_code = 36 ;

    _fs_load_by_cluster_code(kernel_cluster_code,0x1000,0x0000);
    kernel_start();
    return 0;
}

// 使用第一个簇号，加载多个簇直到遇到结束扇区
u16 _fs_load_by_cluster_code(u16 cluster_code, u16 segment, u16 offset ){
    int size = 0;
    while (0x0002 <= cluster_code && cluster_code <= 0xFFEF){
        size += 512*cluster2sector;
        int sector_number = first_cluster_by_sector + (cluster_code-2)*cluster2sector;
        read_n_sector(sector_number, cluster2sector, segment, offset);
        offset = offset + cluster2sector*512;
        cluster_code = FAT_table[cluster_code];
    }
    return size;
}


void read_n_sector(u16 sector_code, u16 number,  u16 segment, u16 offset){
    for (int i = 0; i < number; i++){
        int sector = (sector_code+i) % 18+1;
        int mid = (sector_code+i) / 18;
        int cylinder = mid >> 1;
        int head = mid & 1;
        read_sector(head, cylinder,sector,segment, offset + i*512);
    }
    return ;
}