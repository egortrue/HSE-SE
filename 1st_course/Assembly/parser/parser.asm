CODE_SEG segment
    ASSUME CS:CODE_SEG, DS:CODE_SEG, ES:CODE_SEG
org 100h
start: jmp begin
;-------------------------------------------------------

clear_email macro
    local buffer_email_clear
    push CX
    push DI

    xor DI, DI
    mov CX, buffer_email_size
    buffer_email_clear:
        mov buffer_email[DI], 0
        inc DI
    loop buffer_email_clear
    mov buffer_email_pos, 0

    pop DI
    pop CX
endm

write_email_letter macro
    push DI

    mov DI, offset buffer_email
    add DI, buffer_email_pos
    stosb ; AL - letter
    inc buffer_email_pos

    pop DI
endm

print_letter macro letter
	push AX
	push DX

	mov	DL, letter
	mov	AH,	02
	int	21h

	pop	DX
	pop	AX

endm

print_message macro	message
	local msg, nxt

	push AX
	push DX

	mov	DX, offset msg
	mov	AH,	09h
	int	21h

	pop	DX
	pop	AX

	jmp nxt
	msg	DB message,'$'
nxt:
endm

;-------------------------------------------------------
begin:
; Check string of parameters
    mov BL,	ES:[80h] 	; length parameters in psp
    cmp BL,	0
    jne with_param
    jmp	without_param

;-------------------------------------------------------
with_param:
	xor	BH,	BH
	mov	byte ptr [BX+81h], 0
    mov	CL,	BL
    xor CH,	CH      ; CX=CL= длина хвоста
    cld             ; DF=0 - флаг направления вперед
    mov DI, 81h     ; ES:DI-> начало хвоста в PSP
    mov AL, ' '     ; Уберем пробелы из начала хвоста
    repe scasb   	; Сканируем хвост пока пробелы
    dec DI          ; DI-> на первый символ после пробелов

; Open file for read/write
    mov	AX,	3D02h
    mov	DX, DI
    int	21h
    jnc	openOK

    print_message "ERROR: can't open the file (as parameter)"
    print_letter CR
    print_letter LF
    int	20h

openOK:
    mov	file_in_handler, AX

    print_message 'File is opened (as parameter)'
    print_letter CR
    print_letter LF
    jmp	main

;-------------------------------------------------------
without_param:

    print_message 'Input file name >> '
    mov	AH,	0Ah
    mov	DX,	offset file_in_name
    int	21h

    print_letter CR
    print_letter LF

    xor	BH,	BH
    mov	BL,  file_in_name[1]
    mov	file_in_name[BX+2],	0

; Open file for read/write
    mov	AX,	3D02h
    mov	DX, offset file_in_name+2
    int	21h
    jc	openERR
    mov	file_in_handler, AX
    jmp	openOK1

openERR:
    print_message "ERROR: can't open the file"
    print_letter CR
    print_letter LF
    int	20h

openOK1:
    print_message 'File is opened'
    print_letter	CR
    print_letter	LF

;-------------------------------------------------------
main:
; Создание выходного файла
    mov	AH, 3Ch	                  ; Функция CREATE
    mov CX, 0	                  ; Без атрибутов
    mov	DX, offset file_out_name  ; Адрес имени Файла
    int	21h
    mov	file_out_handler, AX	  ; Сохранить дескриптор

; Отступить две строки от начала
mov AL, ' '
mov CX, 20
shift:
    write_email_letter
loop shift
mov AL, LF
write_email_letter
mov AL, CR
write_email_letter
mov AL, LF
write_email_letter
mov AL, CR
write_email_letter
mov	AH, 40h				     ; Функция записи
mov	BX, file_out_handler     ; Дескриптор
mov	CX, buffer_email_pos     ; Число записываемых байтов
lea DX, buffer_email         ; Адрес буфера
int 21h

clear_email

;-------------------------------------------------------
read:
; Пытаемся прочитать 32Кб
    mov	AH, 3Fh		        ; Функция READ
    mov	BX, file_in_handler ; Дескриптор
    mov	CX, 32768		    ; Сколько читать
    lea	DX, buffer_read     ; Сюда читать
    int 21h

; Последние 32Кб ?
    cmp AX, 32768
    je not_last
    inc flag_last
    not_last:

    mov SI, 0
    mov CX, AX ; Кол-во прочитанных символов
    parsing:
        push CX
        mov AL, buffer_read[SI]

        ; first letter
        status1:
            cmp parser_handler, 1
            jne status2
            call is_convinient
            cmp AH, 1
            jne err1
            write_email_letter
            inc parser_handler
            err1:
        jmp next_letter

        ; before @
        status2:
            cmp parser_handler, 2
            jne status3
            call is_convinient
            cmp AH, 1
            jne at_sign
            write_email_letter
            jmp next_letter
            at_sign:
            cmp AL, ATS ; if AL = @
            jne err2
            write_email_letter
            inc parser_handler
            jmp next_letter
            err2:
        jmp go_to_email_start

        ; first letter after @
        status3:
            cmp parser_handler, 3
            jne status4
            call is_convinient
            cmp AH, 1
            jne err3
            write_email_letter
            inc parser_handler
            jmp next_letter
            err3:
        jmp go_to_email_start

        ; letters or dot after @
        status4:
            cmp parser_handler, 4
            jne status5
            call is_convinient
            cmp AH, 1
            jne dot_sign
            write_email_letter
            jmp next_letter
            dot_sign:
            cmp AL, DOT
            jne err4
            write_email_letter
            inc parser_handler
            jmp next_letter
            err4:
        jmp go_to_email_start


        ; first letter after dot
        status5:
            cmp parser_handler, 5
            jne status6
            call is_convinient
            cmp AH, 1
            jne err5
            write_email_letter
            inc parser_handler
            jmp next_letter
            err5:
        jmp go_to_email_start

        ; last letters (or space)
        status6:
            call is_convinient
            cmp AH, 1
            jne last_space
            write_email_letter
            jmp next_letter
            last_space:
            cmp AL, SPACE
            je write
            cmp AL, CR
            jne go_to_email_start
            write:

                inc counter_emails

                ; Вывод в консоль (для наглядности)
                print_letter CR
                mov AH, 09h
                lea DX, buffer_email
                int 21h

                ; Запись email в выходной файл
                mov AL, LF
                write_email_letter
                mov AL, CR
                write_email_letter
                mov	AH, 40h				     ; Функция записи
                mov	BX, file_out_handler     ; Дескриптор
                mov	CX, buffer_email_pos     ; Число записываемых байтов
                lea DX, buffer_email         ; Адрес буфера
                int 21h

        go_to_email_start:
            mov parser_handler, 1
            clear_email
        next_letter:
            pop CX
            inc SI
            cmp SI, CX
            je end_of_buffer
    jmp parsing

end_of_buffer:
    cmp flag_last, 1
    je exit
    jmp read

;-------------------------------------------------------
exit:
; Записать в файл кол-во корректных почт
    mov AH, 42h ; Сместить указатель
    mov BX, file_out_handler
    xor AL, AL  ; в начало файла
    xor CX, CX
    xor DX, DX
    int 21h

    ; Вывод текста в первой строке
    mov	AH, 40h				     ; Функция записи
    mov	BX, file_out_handler     ; Дескриптор
    mov	CX, counter_text_size    ; Число записываемых байтов
    lea DX, counter_text         ; Адрес буфера
    int 21h

    ; Вывод кол-ва корректных почт
    mov	AX,	counter_emails	;	Выводимое число в регисте EAX
	push -1					;	Сохраним признак конца числа
	mov CX, 10				;	Делим на 10
l1:
	xor	DX,	DX	;	Очистим регистр DX
	div	CX	    ;	Делим
	push DX	    ;	Сохраним цифру
	or AX,	AX  ;	Остался 0? (это оптимальнее, чем  cmp	ax,	0 )
	jne	l1		;	нет -> продолжим
l2:
	pop	DX		;	Восстановим цифру
	cmp	DX,	-1	;	Дошли до конца -> выход {оптимальнее: or EDX,dx jl ex}
	je	ex
	add	DX,	'0'	;	Преобразуем число в цифру
    mov counter_emails, DX
    mov	AH, 40h				     ; Функция записи
    mov	BX, file_out_handler     ; Дескриптор
    mov	CX, 1                    ; Число записываемых байтов
    lea DX, counter_emails       ; Адрес буфера
    int 21h
	jmp	l2		;	И продолжим
ex:

; Закрыть все файлы и выйти
    mov AX, 4C00h
    int 21h

;-------------------------------------------------------

; Is letter convinient? (A<->Z or a<->z or 0<->9)
; AL - letter
; AH - ouput (1 - yes, 0 - no)
is_convinient PROC
    xor AH, AH
    cmp AL, 65 ; "A"
    jb not_big_letter
    cmp AL, 90 ; "Z"
    ja not_big_letter
    inc AH
    ret
not_big_letter:
    cmp AL, 97 ; "a"
    jb not_letter
    cmp AL, 122 ; "z"
    ja not_letter
    inc AH
    ret
not_letter:
    cmp AL, 48 ; "0"
    jb not_number
    cmp AL, 57 ; "9"
    ja not_number
    inc AH
    not_number:
    ret
is_convinient ENDP

;-------------------------------------------------------
; Data

LF	   EQU	10   ; Line feed
CR	   EQU	13	 ; Carrage return
SPACE  EQU	32
DOT    EQU  46
ATS    EQU  64   ; @ sign

file_in_name  DB 14, 0, 14 dup(0)
file_out_name DB "emails.txt", 0

file_in_handler  DW ?
file_out_handler DW ?

parser_handler DB 1 ; parser status (1-6)

counter_emails    DW 0
counter_text      DB "Correct emails: "
counter_text_size EQU $ - counter_text

buffer_email       DB 101 dup(0), '$'
buffer_email_pos   DW 0
buffer_email_size EQU 100

flag_last   DB 0
buffer_read DB 32768 dup(0)

;-------------------------------------------------------
CODE_SEG ends
end start
