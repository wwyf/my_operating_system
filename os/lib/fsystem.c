#include "../include/basic.h"
#include "../include/string.h"
#include "../include/stdio.h"
#include "../include/type.h"
#include "../include/fsystem.h"

// FileDescriptor[16];
u16 FAT_table[512];
FileDescriptor root[56];
u16 root_sector;
u16 first_cluster_by_sector;
u16 cluster2sector;

void init_fsystem(){
    cluster2sector = 1;
    root_sector = 37;
    first_cluster_by_sector = root_sector + 1;  
    read_n_sector(root_sector,1, 0x1000, root);
    read_n_sector(1,2,0x1000, FAT_table);// 这个会覆盖掉root吗？
    // TODO: tab表可能不够大，在加载多一些
    // FIXME:在fat表和root数组设置得小一点的时候，似乎加载fat_table的时候会覆盖掉root所在的内存空间，导致后面出现错误。
    // puti(FAT_table[0]);
    // puti(root[0].cluster_number);
    // puts(&root[0], 10);
    // puts(&root[1], 10);
    // puts(&root[2], 10);
}

u16 _fs_find_descriptor_number_by_name(char * file_name){
    int file_name_length = strlen(file_name);
    if (file_name_length >= 11) file_name_length = 11;
    // TODO:这里的10是文件数，需要使用常量代替
    for (int i = 0; i < 10; i++){
        if (!strncmp(&root[i], file_name, file_name_length)){
            return i;
        }
    }
    return -1;
}

u16 _fs_find_cluster_code_by_name(char * file_name){
    int index = _fs_find_descriptor_number_by_name(file_name);
    if (index != -1)
        return root[index].cluster_code;
    return 0;
}

u16 _fs_get_file_size_by_cluster_code(u16 cluster_code){
    int size = 0;
    while (0x0002 <= cluster_code && cluster_code <= 0xFFEF){
        size += 512*cluster2sector;
        int sector_number = first_cluster_by_sector + (cluster_code-2)*cluster2sector;
        cluster_code = FAT_table[cluster_code];
    }
    return size;
}

void _fs_show_file_by_descriptor_number(u16 index){
    int cluster_code = root[index].cluster_code;
    printf("%d, %d\n" , index,cluster_code);
    printf("%s   | %d byte |", (char *)&root[index],
                             _fs_get_file_size_by_cluster_code(cluster_code));
    while (0x0002 <= cluster_code && cluster_code <= 0xFFEF){
        printf("-%d", cluster_code);
        cluster_code = FAT_table[cluster_code];
    }
    printf("-|\n");
    return ;
}

u16 _fs_load_by_cluster_code(u16 cluster_code, void(*program)()){
    int size = 0;
    while (0x0002 <= cluster_code && cluster_code <= 0xFFEF){
        size += 512*cluster2sector;
        int sector_number = first_cluster_by_sector + (cluster_code-2)*cluster2sector;
        read_n_sector(sector_number, cluster2sector, 0x1000, program);
        program = program+cluster2sector*512;
        cluster_code = FAT_table[cluster_code];
    }
    return size;
}


u16 fs_load_by_name(char * file_name, void (*program)()){
    int cluster_code = _fs_find_cluster_code_by_name(file_name);
    if (cluster_code){
        return _fs_load_by_cluster_code(cluster_code, program);
    }
    else {
        return 0;
    }
}


u16 fs_get_file_size(char * file_name){
    int cluster_code = _fs_find_cluster_code_by_name(file_name);
    return _fs_get_file_size_by_cluster_code(cluster_code);
}

void fs_show_root_file_table(){
    printf("%s   | %s | %s\n", "file name ", "file size", "cluster");
    printf("------------------------------------\n\r");
    int file_number = 9;// TODO:文件数量需要设置！
    for(int i = 0; i < file_number; i++){
        _fs_show_file_by_descriptor_number(i);
    }
    return ;
}

void fs_show_file_by_name(char * file_name){
    int descriptor_index = _fs_find_descriptor_number_by_name(file_name);
    _fs_show_file_by_descriptor_number(descriptor_index);
    return ;
}


void fs_show_floppy_infomation(){
    return 0;
}