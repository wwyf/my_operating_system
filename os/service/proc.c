#include "../include/type.h"
#include "../include/basic.h"
#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/global.h"
#include "../include/system_call.h"
#include "../include/fsystem.h"
#include "../include/proc.h"

void schedule_process(){
    cur_process_index = (cur_process_index + 1) % 2;
    cur_process = &process_table[cur_process_index];
}