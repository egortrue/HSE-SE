code_seg segment
    ASSUME CS:CODE_SEG, DS:CODE_SEG, ES:CODE_SEG
	org 100h
    include macro.asm
start:
jmp begin
    prog_name   DB 80 dup(), '$'
    name_len    DW 0
    buffer      DB 0
begin:
    clear_screen

;==================================================================
; name of program in environment --> prog_name

    cld ; left -> right
    mov DS, DS:2Ch ; ENVIRONMENT ADDRESS IN PSP
    mov SI, 0      ; begin of env
    lea DI, buffer ; temp buffer

; read common env information
    env:
        xor BX, BX ; len of str
        str:
            movsb
            dec DI
            inc BX
            mov	DL, CS:[buffer]
            cmp DL, 0
        jne str
        dec BX
        cmp BX, 0
    jne env

; useless counter
    movsb
    dec DI
    movsb
    dec DI

; read program name
    lea DI, prog_name
    xor BX, BX
    name:
        movsb
        mov	DL, CS:prog_name[BX]
        inc BX
        cmp DL, 0
    jne name

; restore default DS
    mov AX, CS
    mov DS, AX
    xor AX, AX

; lenght of name
    dec BX
    mov [name_len], BX

;==================================================================
; PRINT TEXT

    print_letter NewLine
    mov CX, 20
    line1:
        print_letter Space
    loop line1
    print_message "Работает программа "

;++++++++++++++++++++++
; print program name using video-buffer
    mov AX, 0B800h
    mov ES, AX
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

;++++++++++++++++++++++
; print other common text
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
; PRINT FRAME USING VIDEO-BUFFER

    xor AX, AX
animation:
; standart width of frame
    mov BX, 62

; standart width + width of name
    mov CX, [name_len]
    add CX, CX
    add BX, CX

; esli ne chetnoe chislo
    and CX, 1
    cmp CX, 1
    jne not_odd
    inc BX
    not_odd:

; smeshenie ot levogo krai
    mov DI, 18
    add DI, DI

;++++++++++++++++++++++
; ASCII corners

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
; Vertical lines
    mov CX, 2
    mov AL, 0BAh
vertical:
    add DI, 80*2
    mov ES:[DI], AX     ; left line
    mov ES:[DI+BX], AX  ; right line
loop vertical
    sub DI, 2*80*2

;++++++++++++++++++++++
; Horizontal lines
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
; Change color
    inc AH
    and AH, 0Fh

;++++++++++++++++++++++
; Waiting for animation
    mov CX, 15
wait:
    push CX
    mov CX, 0FFFFh
    loop $
    pop CX
loop wait

;++++++++++++++++++++++
; Press button to exit
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
