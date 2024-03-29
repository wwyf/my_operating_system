my_core_header_address equ 1000h
core_header_data_segment equ 0100h
core_code_entry_header_offset equ 0x2A
core_code_segment_header_offset equ 0x2C
core_data_segment_header_offset equ 0x2E
core_stack_segment_header_offset equ 0x30
core_stack_length equ 256
core_entry_header_offset equ 0x32
user1_program_address equ 0x2000
user2_program_address equ 0x3000
user3_program_address equ 0x4000
user4_program_address equ 0x5000

user1_bound_x_up equ 1 
user1_bound_x_down equ 12
user1_bound_y_left equ 1
user1_bound_y_right equ 39 

user2_bound_x_up equ 1 
user2_bound_x_down equ 12
user2_bound_y_left equ 40
user2_bound_y_right equ 78 

user3_bound_x_up equ 13 
user3_bound_x_down equ 23
user3_bound_y_left equ 1
user3_bound_y_right equ 39

user4_bound_x_up equ 13 
user4_bound_x_down equ 23
user4_bound_y_left equ 40
user4_bound_y_right equ 78 