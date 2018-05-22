#include <string.h>
#include <type.h>

size_t com_strlenk(char * str){
    uint32_t len = 0;
    while (str[len] != 0){
        len++;
    }
    return len;
}

void strncpyk(char * dest, char * src, uint32_t size){
    for (int i = 0; i < size; i++){
        dest[i] = src[i];
    }
    dest[size] = '\0';
}

int strcmpk(char * lhs, char * rhs){
    int lhs_len = strlenk(lhs);
    int rhs_len = strlenk(rhs);
    if (lhs_len != rhs_len){
        return 1;
    }
    for (int i = 0; i < lhs_len; i++){
        if (lhs[i] != rhs[i])
            return 1;
    }
    return 0;
}

int strncmpk(char * lhs, char * rhs, int size){
    for (int i = 0; i < size; i++){
        if (lhs[i] != rhs[i])
            return 1;
    }
    return 0;
}