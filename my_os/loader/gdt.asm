%include"../include/pm.inc"
;##############################################################
; GDT  Table
;##############################################################
;                              段基址,       段界限     , 属性
LABEL_GDT:   	   Descriptor       0,               0, 0           ; 空描述符
LABEL_DESC_KERNEL: Descriptor  10000h,          10000h, DA_C + DA_32; 非一致代码段
LABEL_DESC_VIDEO:  Descriptor 0B8000h,          0ffffh, DA_DRW	    