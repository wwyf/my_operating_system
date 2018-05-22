#include <type.h>

void memcpy(char * src, char * dest, uint32_t size){
    int count = size;
    while(count--){
        *dest = *src;
        dest = dest + 1;
        src = src + 1;
    }
}
