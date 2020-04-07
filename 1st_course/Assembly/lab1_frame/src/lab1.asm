code_seg segment
    ASSUME  CS:CODE_SEG,DS:CODE_SEG,ES:CODE_SEG
	org 100h
    include macro.asm
start:
    jmp begin
    prog_name   DB 80 dup(), '$'
    name_len    DW 0
    buffer      DB 0, '$'
begin:
	clear_screen

;==================================================================
; To know the name of programm using environment --> prog_name

xor BX, BX
mov CX, 200
mov SI, 0
cld
environment:

    mov DS, DS:2Ch ; ENVIRONMENT ADDRESS IN PSP
    lea DI, buffer
    movsb

    push SI
    push DI

    mov AX, CS
    mov DS, AX

    lea SI, buffer
    lodsb

    cmp AL, 0 ; first zero (ASCIZ)
    jne next

    pop DI
    pop SI

    lea DI, buffer
    movsb

    push SI
    push DI

    mov AX, CS
    mov DS, AX

    lea SI, buffer
    lodsb

    cmp AL, 0 ; second zero (end of ENVIRONMENT)
    je exit

    next:
    pop DI
    pop SI

    mov AX, CS
    mov DS, AX

    inc BX
loop environment
exit:

mov CX, 50
xor AX, AX

mov DS, DS:2Ch
mov SI, BX
add SI, 4
lea DI, prog_name
xor BX, BX
xor AX, AX
path:
    lodsb
    cmp AL, 0
    je exit1
    dec SI
    movsb
    inc BX
loop path
exit1:
mov AX, CS
mov DS, AX

name_len_init:
    cmp BX, 0
    je name_len_inited
    inc name_len
    dec BX
    loop name_len_init
name_len_inited:


;==================================================================
; Print text using video-buffer
    mov AX, 0B800h
    mov ES, AX

    ; common text
    print_letter NewLine
    mov CX, 20
    line1:
        print_letter Space
    loop line1
    print_message "Работает программа "

    ; programm name
    mov AH, Yellow ; attribut
    lea SI, prog_name
    mov DI, 80*2+78
    mov CX, [name_len]
    cld
    print_text:
        print_letter Space
        lodsb
        stosw
    loop print_text

    ; other text
    print_message ", которая"
    print_letter NewLine
    mov CX, 20
    line2:
        print_letter Space
    loop line2
    print_message "выводит данное сообщение !"

    print_letter NewLine
    print_letter NewLine
    print_letter NewLine
    print_letter CRet
    print_message "Нажмите любую клавишу, чтобы выйти..."
;==================================================================
; Print frame using video-buffer

    xor AX, AX
    mov AH, 0  ; attribut
animation:
    ; width of frame
    mov BX, 62

    ; add more width because of name
    mov CX, [name_len]
    add CX, CX
    add BX, CX
    ;esli ne chetnoe chislo
    and CX, 1
    cmp CX, 1
    jne not_odd
    inc BX
    not_odd:

    ; smeshenie ot levogo  krai
    mov DI, 18
    add DI, DI

;++++++++++++++++++++++
; ASCII corners (ygli)

; top-left corner
    mov AL, 0C9h
    mov ES:[DI], AX

; top-right corner
    mov AL, 0BBh
    mov ES:[DI+BX], AX

; bottom-left corner
    mov AL, 0C8h
    mov ES:[DI + 80*2*3], AX

; bottom-right corner
    mov AL, 0BCh
    mov ES:[DI+BX + 80*2*3], AX

;++++++++++++++++++++++
; vertical lines
    mov CX, 2
    mov AL, 0BAh
vertical:

    add DI, 80*2
    mov ES:[DI], AX     ; left line
    mov ES:[DI+BX], AX  ; right line
loop vertical
    sub DI, 2*80*2

;++++++++++++++++++++++
; horizontal lines
    mov CX, BX
    sub CX, 2
    mov AL, 0CDh
horizontal:

    add DI, 2
    mov ES:[DI], AX          ; top line
    mov ES:[DI + 80*2*3], AX ; bottom line
    dec CX

loop horizontal

;++++++++++++++++++++++
; change color
    inc AH
    and AH, 0Fh

;++++++++++++++++++++++
; waiting for animation
; not yet
;++++++++++++++++++++++
; jump to animation, while no one char be in input buffer
    push AX
    mov AH, 0Bh
    int 21h
    cmp AL, 0
    pop AX
je animation

;==================================================================
; Close all files and exit
    mov  AX, 4C00h
    int  21h

code_seg ends
end start
