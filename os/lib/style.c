#include "../include/type.h"

void move_name(){
    u16 *text_line = 1920*2;
    // text_line 0-79
    // text_line[80]
    u16 last_char = text_line[79];
    for (int i =78; i >= 0; i--){
        text_line[i+1] = text_line[i];
    }
    text_line[0] = last_char;
}