;+--------------------------------------------------------------------------
; Эта TSR программа выводит квадрат с помощью видеобуфера
; Чтобы появился квадрат необходимо нажать любую из клавиш управления
;
; Управление:
; F12 - Заново отрисовать квадрат
; F11 - Увеличить квадрат
; F10 - Уменьшить квадрат
; F9  - Сменить цвет (синий - по умолчанию)
; Стрелки - Перемещение квадрата по экрану
;
; Выгрузка:
; >имяпрог -exit (например: lab2 -exit)
;+--------------------------------------------------------------------------
code_seg segment
    ASSUME CS:CODE_SEG, DS:CODE_SEG, ES:CODE_SEG
org 100h
start:
    jmp boot ; Прыжок на инициализацию
;===========================================================
; Резидентная часть:

old_2Fh DD ?
old_09h DD ?

choose_key DB 0

box_pos   DW 12*160+76 ; Положение квадрата (верхний левый угол)
box_size  DW 2         ; Размер квадрата
box_color DB 1         ; Цвет квадрата

;===========================================================

print_box PROC far
    ; укажем адрес видеобуфера
    mov AX, 0B800h
    mov ES, AX
    ; установим символ и его цвет
    mov AH, CS:box_color
    mov AL, 0DBh
    ; заполняем буфер
    mov DI, CS:box_pos
    mov CX, CS:box_size
    row:
        push CX
        push DI
        mov CX, CS:box_size
        add CX, CX
        column:
            mov ES:[DI], AX ; заносим (символ+цвет) в буфер
            add DI, 2
        loop column
        pop DI
        add DI, 160
        pop CX
    loop row
    retf
print_box ENDP

;===========================================================

new_09h PROC far
    pushf
	push AX

    ; Введем скен-код
    in AL, 60h
    mov CS:choose_key, AL

    ; Используемые клавишы
    cmp CS:choose_key, 58h  ; Это скен-код <F12>
    je hotkey
    cmp CS:choose_key, 57h  ; Это скен-код <F11>
    je hotkey
    cmp CS:choose_key, 44h  ; Это скен-код <F10>
    je hotkey
    cmp CS:choose_key, 43h  ; Это скен-код <F9>
    je hotkey
    cmp CS:choose_key, 48h  ; Это скен-код <стрелка вверх>
    je hotkey
    cmp CS:choose_key, 4Bh  ; Это скен-код <стрелка влево>
    je hotkey
    cmp CS:choose_key, 50h  ; Это скен-код <стрелка вниз>
    je hotkey
    cmp CS:choose_key, 4Dh  ; Это скен-код <стрелка вправо>
    je hotkey


    mov CS:choose_key, 0
    pop  AX
	popf
    jmp dword ptr CS:[old_09h]

hotkey:
    sti              ; Не будем мешать таймеру
    in  AL, 61h      ; Введем содержимое порта B
    or  AL, 80h      ; Установим старший бит
    out 61h, AL      ; и вернем в порт B.
    and AL, 7Fh      ; Снова разрешим работу клавиатуры,
    out 61h, AL      ; сбросив старший бит порта B.

    ; Cохранение используемых регистров в стеке
    push BX
    push CX
    push DX
    push DS
    push ES

    ; Настройка DS на наш сегмент, т.е DS=CS
    push CS
    pop DS

    ; Обработка нажатой клавишы
    cmp CS:choose_key, 57h  ; Это скен-код <F11>
    jne key_f10
    cmp CS:box_size, 8
    je done1
    inc CS:box_size
    done1:
    jmp done

    key_f10:
    cmp CS:choose_key, 44h  ; Это скен-код <F10>
    jne key_f9
    cmp CS:box_size, 1
    je done2
    dec CS:box_size
    done2:
    jmp done

    key_f9:
    cmp CS:choose_key, 43h  ; Это скен-код <F9>
    jne key_up
    inc CS:box_color
    jmp done

    key_up:
    cmp CS:choose_key, 48h  ; Это скен-код <стрелка вверх>
    jne key_down
    cmp CS:box_pos, 160
    jb done3
    sub CS:box_pos, 160
    done3:
    jmp done

    key_down:
    cmp CS:choose_key, 50h  ; Это скен-код <стрелка вниз>
    jne key_left
    cmp CS:box_pos, 24*160-1
    ja done4
    add CS:box_pos, 160
    done4:
    jmp done

    key_left:
    cmp CS:choose_key, 4Bh  ; Это скен-код <стрелка влево>
    jne key_right
    sub CS:box_pos, 2
    jmp done

    key_right:
    cmp CS:choose_key, 4Dh  ; Это скен-код <стрелка вправо>
    jne done
    add CS:box_pos, 2

    done:
    ; При нажатии любой из кнопок управления
    ; всегда очиститься экран и покажется квадрат
    mov AX, 0003h
    int 10h
    call print_box

    ; Восстановление регистров из стека
    pop ES
    pop	DS
    pop DX
    pop CX
    pop BX

    ; Пошлем приказ EOI
    cli
    mov AL, 20h
    out 20h, AL
    pop AX
	popf
    iret
new_09h ENDP

;===========================================================

new_2Fh PROC far

    ; Наш номер?
    cmp AH, 0C7h
    jne pass_2Fh

    ; Повторная загрузка ?
    cmp AL, 00h
    je inst

    ; Выгрузка ?
    cmp AL, 01h
    je unins

    ; Неизвестная подфункция - на выход
    jmp short pass_2Fh

inst:
    mov AL, 0FFh ; Сообщим о невозможности повторной установки
    iret

pass_2Fh:
    jmp dword PTR CS:[old_2Fh]

; -------------- Проверка - возможна ли выгрузка программы из памяти ? ------

unins:
    push BX
    push CX
    push DX
    push ES

    ; Проверить вектор 09h
    mov CX, CS
    mov AX, 3509h ; Функция 35h в AL - номер прерывания.
    int 21h       ; Возврат: вектор в ES:BX

    mov DX, ES
    cmp CX, DX
    jne Not_remove

    cmp BX, offset CS:new_09h
    jne Not_remove

    ; Проверить вектор 2Fh
    mov AX, 352Fh  ; Функция 35h в AL - номер прерывания.
    int 21h        ; Возврат: вектор в ES:BX

    mov DX,ES
    cmp CX,DX
    jne Not_remove

    cmp BX, offset CS:new_2Fh
    jne Not_remove

; ---------------------- Выгрузка программы из памяти ---------------------
    push DS

    ; Заполнение вектора 09h старым содержимым
    lds DX, CS:old_09h
    mov AX, 2509h
    int 21h

    ; Заполнение вектора 2Fh старым содержимым
    lds DX, CS:old_2Fh
    mov AX, 252Fh
    int 21h

    pop DS

    ; Очистка окружения
    mov ES, CS:2Ch  ; ES -> env
    mov AH, 49h     ; Функция освобождения блока памяти
    int 21h

    ; Выгрузим саму программу
    mov AX, CS
    mov ES, AX      ; ES -> PSP
    mov AH, 49h     ; Функция освобождения блока памяти
    int 21h

    ; Признак успешной выгрузки
    mov AL, 0Fh
    jmp short pop_ret

Not_remove:
    mov AL, 0F0h ; Признак - выгружать нельзя

pop_ret:
    pop     ES
    pop     DX
    pop     CX
    pop     BX
    iret
new_2Fh ENDP

;===========================================================
; Часть инициализации:
boot:

    mov CL, ES:80h   ; Длина хвоста в PSP
    cmp CL, 0        ; Длина хвоста == 0?

    ; Программа запущена без параметров ?
    ; да - попробуем установить
    je check_install

    ; нет - читаем хвост
    xor CH, CH   ; CX=CL = длина хвоста
    cld          ; DF=0  - флаг чтения слева направо
    mov DI, 81h  ; ES:DI - начало хвоста в PSP
    lea SI, key  ; DS:SI - поле key

    ; Уберем пробелы из начала хвоста
    mov AL, ' '
    repe scasb

    ; Сравниваем введенный хвост с ожидаемым
    dec DI        ; DI -> на первый символ после пробелов
    mov CX, 5     ; ожидаемая длина команды '-exit'
    repe cmpsb

    jne check_install ; Неизвестная команда - пробуем установить
    inc f_exit        ; Иначе устанавливаем флаг выхода

;===========================================================
check_install:

    ; Мультиплексное прерывание
    mov AX, 0C700h   ; AH = C7h - номер процесса
    int 2Fh          ; AL = 00h - дать статус установки процесса

    ; Проверка установки
    cmp AL, 0FFh ; 0FFh = установлен
    je installed

    ; Проверка флага выхода без установленной программы
    cmp f_exit, 1
    je exit_error

    ;------------- Установка --------------

    ; Получим вектор прерывания 2Fh
    mov AX, 352Fh
    int 21h

    ; Запомним старый вектор
    mov word ptr old_2Fh,   BX
    mov word ptr old_2Fh+2, ES

    ; Установим новый вектор прерывания
    lea DX, new_2Fh ; Получим нового обработчика
    mov AX, 252Fh   ; AL - номер прерывания
    int 21h         ; DS:DX - указатель программы обработки прерывания

    ; Получим вектор прерывания 09h
    mov AX, 3509h
    int 21h

    ; Запомним старый вектор
    mov word ptr old_09h,   BX
    mov word ptr old_09h+2, ES

    ; Установим новый вектор прерывания
    lea DX, new_09h ; Получим нового обработчика
    mov AX, 2509h   ; AL - номер прерывания
    int 21h         ; DS:DX - указатель программы обработки прерывания

    ; Сообщение об установке
    lea DX, msg1
    call print

    mov DX, offset boot  ;   оставить программу ...
    int 27h              ;   ... резидентной и выйти

;===========================================================
installed:
    cmp f_exit, 1     ; Запрос на выгрузку установлен?
    je  uninstall     ; Да, на выгрузку

    ; Cообщение: уже установленно!
    lea DX, msg
    call print
    int 20h

;===========================================================
uninstall:
    ; Мультиплексное прерывание
    mov AX, 0C701h ; AH=0C7h номер процесса C7h
    int 2Fh        ; подфункция 01h - выгрузка

    cmp AL, 0F0h
    je  not_sucsess
    cmp AL, 0Fh
    jne not_sucsess

    ; Сообщение о выгрузке
    lea DX, msg2
    call print
    int 20h

;===========================================================
; Сообщение, что выгрузка невозможна
not_sucsess:
    lea DX, msg3
    call print
    int 20h

;===========================================================
; Сообщение: программы нет, а пользователь дает команду выгрузки
exit_error:
    lea DX, msg4
    call print
    int 20h

;===========================================================

key     DB  '-exit'
f_exit  DB  0
msg     DB  'already '
msg1    DB  'installed',13,10,'$'
msg4    DB  'just '
msg3    DB  'not '
msg2    DB  'uninstalled',13,10,'$'

print PROC near
    mov AH, 09h
    int 21h
    ret
print ENDP

;===========================================================

code_seg ends
end start
