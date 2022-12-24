#include "stdafx.h"
#define SEPSTREMP  "\n;------------------------------\n"
#define SEPSTR(x)  "\n;----------- " + string(x) + " ------------\n"


#define BEGIN ".586\n"\
".model flat, stdcall\n"\
"includelib libucrt.lib\n"\
"includelib kernel32.lib\n"\
"includelib \"../Debug/Lib.lib\"\n"\
"ExitProcess PROTO:DWORD \n"\
".stack 4096\n"

#define END "INVOKE ExitProcess,0\nEXIT_DIV_ON_NULL:\nINVOKE ExitProcess,-1\nmain ENDP\nend main"

#define EXTERN "ExitProcess PROTO :DWORD\n\
outn PROTO : SDWORD\n\
outw PROTO : DWORD\n\
len PROTO : DWORD\n\
comp PROTO : DWORD, : DWORD\n\
stcmp PROTO : DWORD, : DWORD\n"



#define ITENTRY(x)  tables.idtable.table[tables.lextable.table[x].idxTI]
#define LEXEMA(x)   tables.lextable.table[x].lexema


#define CONST ".const\n\t\tnewline byte 13, 10, 0"
#define DATA ".data\n\t\ttemp sdword ?\n\t\tbuffer byte 256 dup(0)\n"
#define CODE ".code\n\
int_to_char PROC uses eax ebx ecx edi esi,\n\
pstr: dword,\n\
intfield : sdword\n\
mov edi, pstr\n\
mov esi, 0\n\
mov eax, intfield\n\
cdq\n\
mov ebx, 10\n\
idiv ebx\n\
test eax, 80000000h\n\
jz plus\n\
neg eax\n\
neg edx\n\
mov cl, '-'\n\
mov[edi], cl\n\
inc edi\n\
plus :\n\
push dx\n\
inc esi\n\
test eax, eax\n\
jz fin\n\
cdq\n\
idiv ebx\n\
jmp plus\n\
fin :\n\
mov ecx, esi\n\
write :\n\
pop bx\n\
add bl, '0'\n\
mov[edi], bl\n\
inc edi\n\
loop write\n\
ret\n\
int_to_char ENDP\n"


namespace Gener
{
	void CodeGeneration(Lex::LEX& tables, Parm::PARM& parm, Log::LOG& log);
};