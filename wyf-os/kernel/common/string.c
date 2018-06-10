#include <common/string.h>
#include <type.h>

PUBLIC size_t com_strlen(char * str){
    uint32_t len = 0;
    while (str[len] != 0){
        len++;
    }
    return len;
}

PUBLIC void com_strncpy(char * dest, char * src, uint32_t size){
    for (int i = 0; i < size; i++){
        dest[i] = src[i];
    }
    dest[size] = '\0';
}

PUBLIC int com_strcmp(char * lhs, char * rhs){
    int lhs_len = com_strlen(lhs);
    int rhs_len = com_strlen(rhs);
    if (lhs_len != rhs_len){
        return 1;
    }
    for (int i = 0; i < lhs_len; i++){
        if (lhs[i] != rhs[i])
            return 1;
    }
    return 0;
}

PUBLIC int com_strncmp(char * lhs, char * rhs, int size){
    for (int i = 0; i < size; i++){
        if (lhs[i] != rhs[i])
            return 1;
    }
    return 0;
}

PUBLIC void com_memset(void* p_dst, char ch, size_t size){
    char * dest = (char *)p_dst;
    for (int i = 0; i < size; i++){
        *dest = ch;
        dest++;
    }
    return ;
}