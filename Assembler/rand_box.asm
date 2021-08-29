;+--------------------------------------------------------------------------
; 2020 (c) Трухин Егор
; Эта программа выводит фигуры разных размеров с помощью видеобуфера
; Используя псевдорандомность, связанная с локальным временем
; Чтобы выйти из программы нажмите любую клавишу клавиатуры
;+--------------------------------------------------------------------------
code_seg segment
    ASSUME CS:CODE_SEG, DS:CODE_SEG, ES:CODE_SEG
org 100h
start:

    ; clear screen
    mov AX, 0003h
    int 10h

    ; get time (current second)
    mov AH, 2Ch
    int 21h
    mov time, DH

    box:

        push CX

        ; +y
        xor AX, AX
        mov box_pos, 12*160+76
        call rand8
        and AL, 12
        mov AH, 160
        mul AH
        add box_pos, AX

        ; -y
        xor AX, AX
        call rand8
        and AL, 15
        mov AH, 160
        mul AH
        sub box_pos, AX

        ; +x
        xor AX, AX
        call rand8
        and AL, 20
        mov AH, 2
        mul AH
        add box_pos, AX

        ; -x
        xor AX, AX
        call rand8
        and AL, 50
        mov AH, 2
        mul AH
        sub box_pos, AX

        ; rand size x
        xor AX, AX
        call rand8
        and AX, 20
        add AX, 1
        mov box_x, AX

        ; rand size y
        xor AX, AX
        call rand8
        and AX, 10
        add AX, 1
        mov box_y, AX

        ; rand color
        call rand8
        and AL, 15
        mov box_color, AL

        ; print box
        call print_box

        ; waiting
        mov CX, 8
        waiting:
            push CX
            mov CX, 0FFFFh
            loop $
            pop CX
        loop waiting

        ; update current time
        mov AH, 2Ch
        int 21h
        mov time, DH

        ; press button case
        push AX
        mov AH, 0Bh
        int 21h
        cmp AL, 0
    jne exit
    jmp box
exit:

;==================================================================
; Close all files and exit
    mov  AX, 4C00h
    int  21h
;==================================================================
print_box PROC near
    ; укажем адрес видеобуфера
    mov AX, 0B800h
    mov ES, AX
    ; установим символ и его цвет
    mov AH, box_color
    mov AL, 0DBh
    ; заполняем буфер
    mov DI, box_pos
    mov CX, box_y
    row:
        push CX
        push DI
        mov CX, box_x
        column:
            mov ES:[DI], AX ; заносим (символ+цвет) в буфер
            add DI, 2
        loop column
        pop DI
        add DI, 160
        pop CX
    loop row
    ret
print_box ENDP
;==================================================================
; rand8
; Возвращает случайное 8-битное число в AL.
; Переменная seed должна быть инициализирована заранее,
; например из области данных BIOS, как в примере для конгруэнтного генератора.
rand8 proc near
	mov AX,	word ptr seed ; Считать последнее случайное число.
	test AX, AX		      ; Проверить его, если это -1,
	js fetch_seed		  ; Функция еще ни разу не вызывалась и надо создать
randomize:
	mov	AX,	word ptr seed
	mov	CX,	8
newbit:	mov	BX,	AX
	and	BX,	002Dh
	xor	BH,	BL
	clc
	jpe	shift
	stc
shift:
    rcr	AX,	1
	loop newbit
	mov	word  ptr	seed,	AX
	xor	AH,	AH
	mov	prb, AX
    add AL, time
	ret
fetch_seed:
	push DS
	push 0040h
	pop	DS
	mov	AX,	word ptr DS:006Ch+2	; Считать двойное слово из области
	pop	DS					    ; данных BIOS по адресу	0040:006С - текущее число
	jmp	short randomize			; тактов таймера.
rand8 endp
;==================================================================
box_pos   DW 12*160+76 ; Положение квадрата (верхний левый угол)
box_x     DW 1         ; Размер квадрата по x
box_y     DW 1         ; Размер квадрата по y
box_color DB 1         ; Цвет квадрата

time DB 0
seed DW -1
prb	DW	0
;==================================================================
code_seg ends
end start
