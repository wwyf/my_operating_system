#include "../include/type.h"

u32 strlen(char * str){
    u32 len = 0;
    while (str[len] != 0){
        len++;
    }
    return len;
}