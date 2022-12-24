.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib "../Debug/Lib.lib"
ExitProcess PROTO:DWORD 
.stack 4096

ExitProcess PROTO :DWORD
outn PROTO : SDWORD
outw PROTO : DWORD
len PROTO : DWORD
comp PROTO : DWORD, : DWORD
stcmp PROTO : DWORD, : DWORD

.const
		newline byte 13, 10, 0
L1 byte 'Sum:', 0
L2 sdword 2
L3 sdword 1
L4 byte 'Max:', 0
L5 sdword 5
L6 byte 'Hello World!', 0
L7 byte 'Hello', 0
L8 sdword 7
L9 sdword 3
L10 sdword 255
.data
		temp sdword ?
		buffer byte 256 dup(0)

maxres sdword 0
test1 sdword 0
test2 sdword 0
hi dword ?
h1 dword ?
a sdword 0
a1 sdword 0
test3 sdword 0
test4 sdword 0
test5 sdword 0
test6 sdword 0
hex sdword 0
left dword ?
rig dword ?
result sdword ?
result_str byte 4 dup(0)
.code
int_to_char PROC uses eax ebx ecx edi esi,
pstr: dword,
intfield : sdword
mov edi, pstr
mov esi, 0
mov eax, intfield
cdq
mov ebx, 10
idiv ebx
test eax, 80000000h
jz plus
neg eax
neg edx
mov cl, '-'
mov[edi], cl
inc edi
plus :
push dx
inc esi
test eax, eax
jz fin
cdq
idiv ebx
jmp plus
fin :
mov ecx, esi
write :
pop bx
add bl, '0'
mov[edi], bl
inc edi
loop write
ret
int_to_char ENDP


;----------- max ------------
max PROC,
	maxx : sdword, maxy : sdword  
; --- save registers ---
push ebx
push edx
; ----------------------
push maxx

pop ebx
mov left, ebx

push maxy

pop ebx
mov rig, ebx

mov edx, left
cmp edx, rig

jg right1
jl wrong1
 right1:
push maxx

pop ebx
mov maxres, ebx


jmp next1
wrong1:
push maxy

pop ebx
mov maxres, ebx

next1:
; --- restore registers ---
pop edx
pop ebx
; -------------------------
mov eax, maxres
ret
max ENDP
;------------------------------


;----------- sum ------------
sum PROC,
	sumx : sdword, sumy : sdword  
; --- save registers ---
push ebx
push edx
; ----------------------
push sumx
push sumy
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov sumx, ebx

; --- restore registers ---
pop edx
pop ebx
; -------------------------
mov eax, sumx
ret
sum ENDP
;------------------------------


;----------- MAIN ------------
main PROC

INVOKE outw, offset L1


push L3
push L2
call sum
push eax

pop ebx
mov test1, ebx

mov eax, test1
mov result, eax	
INVOKE int_to_char, offset result_str, result
INVOKE outw, offset result_str


INVOKE outw, offset L4


push L5
push L2
call max
push eax

pop ebx
mov test2, ebx

mov eax, test2
mov result, eax	
INVOKE int_to_char, offset result_str, result
INVOKE outw, offset result_str

mov hi, offset L6
mov h1, offset L7

push hi
push hi
call comp
push eax

pop ebx
mov a, ebx


push h1
push hi
call comp
push eax

pop ebx
mov a1, ebx


INVOKE outw, h1


INVOKE outw, hi

mov eax, a
mov result, eax	
INVOKE int_to_char, offset result_str, result
INVOKE outw, offset result_str

mov eax, a1
mov result, eax	
INVOKE int_to_char, offset result_str, result
INVOKE outw, offset result_str

push L8
push L9

	pop ebx
	pop eax
	test ebx, ebx
	jz EXIT_DIV_ON_NULL
	cdq
	mov	edx, 	0
	idiv ebx
	push edx
pop ebx
mov test3, ebx

push L8
push L9
pop ebx
pop eax
sub eax, ebx
push eax

pop ebx
mov test4, ebx

push L8
push L9

	pop ebx
	pop eax
	test ebx, ebx
	jz EXIT_DIV_ON_NULL
	cdq
	mov	edx, 	0
	idiv ebx
	push eax
pop ebx
mov test5, ebx

push L8
push L9
pop ebx
pop eax
imul eax, ebx
push eax

pop ebx
mov test6, ebx

mov eax, test3
mov result, eax	
INVOKE int_to_char, offset result_str, result
INVOKE outw, offset result_str

mov eax, test4
mov result, eax	
INVOKE int_to_char, offset result_str, result
INVOKE outw, offset result_str

mov eax, test5
mov result, eax	
INVOKE int_to_char, offset result_str, result
INVOKE outw, offset result_str

mov eax, test6
mov result, eax	
INVOKE int_to_char, offset result_str, result
INVOKE outw, offset result_str

push L10

pop ebx
mov hex, ebx

mov eax, hex
mov result, eax	
INVOKE int_to_char, offset result_str, result
INVOKE outw, offset result_str

INVOKE ExitProcess,0
EXIT_DIV_ON_NULL:
INVOKE ExitProcess,-1
main ENDP
end main
