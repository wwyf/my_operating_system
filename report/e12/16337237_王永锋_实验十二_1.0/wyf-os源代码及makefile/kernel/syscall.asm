global	sendrec
extern g_cur_proc
_NR_sendrec equ 1
INT_VECTOR_SYS_CALL equ 0x80
; ====================================================================================
;                  sendrec(int function, int src_dest, MESSAGE* msg);
; NOTICE:修改了寄存器，有副作用！	====================================================================================
; Never call sendrec() directly, call send_recv() instead.
sendrec:
	mov	eax, _NR_sendrec
	mov	ebx, [esp + 4]	; function
	mov	ecx, [esp + 8]	; src_dest
	mov	edx, [esp + 12]	; p_msg
	mov esi, [g_cur_proc]
	int	INT_VECTOR_SYS_CALL
	ret
