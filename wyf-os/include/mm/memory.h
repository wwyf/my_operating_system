#ifndef _MEMORY_H_
#define _MEMORY_H_

void free_page();

void copy_page_tables();

void get_empty_tables();

void free_page_tables();

void put_page();

void do_wp_page();

void do_no_page();

void do_page_fault();

#endif // !_MEMORY_H_