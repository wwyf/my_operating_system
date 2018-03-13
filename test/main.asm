extern add
extern printf
extern exit

global _start

section .data
  format db "%d", 10, 0
section .text

_start:
    push  6
    push  2
    call  add     ; add(2, 6)

    push  eax
    push  format
    call  printf  ; printf(format, eax)

    push  0
    call exit     ; exit(0)