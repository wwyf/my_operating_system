#include <type.h>

void com_memcpy(char * src, char * dest, uint32_t size){
    int count = size;
    while(count--){
        *dest = *src;
        dest = dest + 1;
        src = src + 1;
    }
}
