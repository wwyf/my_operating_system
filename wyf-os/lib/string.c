#include "../include/type.h"

size_t strlen(char * str){
    u32 len = 0;
    while (str[len] != 0){
        len++;
    }
    return len;
}

void strncpy(char * dest, char * src, u32 size){
    for (int i = 0; i < size; i++){
        dest[i] = src[i];
    }
    dest[size] = '\0';
}

int strcmp(char * lhs, char * rhs){
    int lhs_len = strlen(lhs);
    int rhs_len = strlen(rhs);
    if (lhs_len != rhs_len){
        return 1;
    }
    for (int i = 0; i < lhs_len; i++){
        if (lhs[i] != rhs[i])
            return 1;
    }
    return 0;
}

int strncmp(char * lhs, char * rhs, int size){
    for (int i = 0; i < size; i++){
        if (lhs[i] != rhs[i])
            return 1;
    }
    return 0;
}