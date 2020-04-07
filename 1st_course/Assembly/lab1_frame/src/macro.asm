CRet		EQU		13	 ; Carrage return
NewLine		EQU		10
Space	    EQU		32
Black       EQU      0
Yellow      EQU    0Eh
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
clear_screen	macro ;  cls

	push AX

	mov AX, 3
	int 10h

	pop AX

endm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
print_letter	macro	letter

	push	AX
	push	DX

	mov	DL, letter
	mov	AH,	02
	int	21h

	pop	DX
	pop	AX

endm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
print_message	macro	message
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
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
open_file	macro	file_name
	local name, nxt

	push AX
	push DX

	mov	AX,	3D02h		; Open file for read/write
	mov	DX, offset name
	int	21h

	pop	DX
	pop	AX

	jmp	nxt
	name DB file_name, 0

nxt:
endm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
