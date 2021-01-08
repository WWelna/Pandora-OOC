; Simple String Functions in Assembly
;  
; Copyright (C) 2012, William H. Welna All rights reserved.
;
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions are met:
;     ; Redistributions of source code must retain the above copyright
;       notice, this list of conditions and the following disclaimer.
;     ; Redistributions in binary form must reproduce the above copyright
;       notice, this list of conditions and the following disclaimer in the
;       documentation and/or other materials provided with the distribution.
;
; THIS SOFTWARE IS PROVIDED BY William H. Welna ''AS IS'' AND ANY
; EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
; WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
; DISCLAIMED. IN NO EVENT SHALL William H. Welna BE LIABLE FOR ANY
; DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
; (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
; ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
; (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
; SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

section .txt
; global declarations
global myxor
global myxorr
global myinv
global mycpy
global myset
global myzero
global mycmp
global mylen
global mycat
global find

myinv: ; void __cdecl myinv(void *, unsigned int);
	push ebp
	mov ebp, esp
	push edi
	push esi
	mov esi, [ebp+8] ; buff
	mov edi, esi; ; dest is src
	mov ecx, [ebp+12] ; len
	mov ah, 0xFF ; invert
	.loop:
		lodsb
		xor al, ah
		stosb
		loop .loop	
	pop esi
	pop edi
	mov esp, ebp
	pop ebp
	ret
myxor: ; void __cdecl myxor(void *, unsigned int, char);
	push ebp
	mov ebp, esp
	push edi
	push esi
	mov esi, [ebp+8] ; buff
	mov edi, esi; ; dest is src
	mov ecx, [ebp+12] ; len
	mov ah, [ebp+16] ; char to xor
	.loop:
		lodsb
		xor al, ah
		stosb
		loop .loop	
	pop esi
	pop edi
	mov esp, ebp
	pop ebp
	ret
myxorr: ; void __cdecl myxorr(char, void *, unsigned int);
	push ebp
	mov ebp, esp
	push edi
	push esi
	mov ah, [ebp+8] ; char to xor
	mov esi, [ebp+12] ; buff
	mov edi, esi; ; dest is src
	mov ecx, [ebp+16] ; len
	.loop:
		lodsb
		xor al, ah
		stosb
		loop .loop
	pop esi
	pop edi
	mov esp, ebp
	pop ebp
	ret
mycpy: ; void __cdecl *mycpy(void *, void *, unsigned int);
	push ebp
	mov ebp, esp
	push edi
	push esi
	mov edi, [ebp+8] ; dest
	mov esi, [ebp+12] ; src
	mov ecx, [ebp+16] ; size
	cld
	rep movsb
	mov eax, [ebp+8]
	pop esi
	pop edi
	mov esp, ebp
	pop ebp
	ret
myset: ; void __cdecl *myset(void *, char, unsigned int);
	push ebp
	mov ebp, esp
	push edi
	push esi
	mov edi, [ebp+8] ; dest
	mov al, [ebp+12] ; char
	mov ecx, [ebp+16] ; size
	cld
	rep stosb
	mov eax, [ebp+8] ; return dest
	pop esi 
	pop edi
	mov esp, ebp
	pop ebp
	ret
myzero: ; void __cdecl *myzero(void *, unsigned int);
	push ebp
	mov ebp, esp
	push edi
	push esi
	mov edi, [ebp+8] ; dest
	mov ecx, [ebp+12] ; size
	xor al, al ; zero!
	cld
	rep stosb
	mov eax, [ebp+8] ; return dest
	pop esi 
	pop edi
	mov esp, ebp
	pop ebp
	ret
mycmp: ; int __cdecl mycmp(void *, void *, unsigned int);
	push ebp
	mov ebp, esp
	push edi
	push esi
	xor eax, eax
	mov esi, [ebp+8] ; string 1
	mov edi, [ebp+12] ; string 2
	mov ecx, [ebp+16] ; size
	cld
	repe cmpsb
	jne .ne
	inc eax  
	.ne:
	pop esi 
	pop edi
	mov esp, ebp
	pop ebp
	ret
mylen: ; int __cdecl mylen(void *);
	push ebp
	mov ebp, esp
	push edi
	push esi
	xor eax, eax ; \0 byte
	mov edi, [ebp+8] ; string
	mov ecx, 0xFFFFFFFF
	cld
	repne scasb
	mov eax, [ebp+8]
	xchg eax, edi
	sub eax, edi
	dec eax ; don't count the \0 byte
	pop esi 
	pop edi
	mov esp, ebp
	pop ebp
	ret
mycat: ; void __cdecl mycat(void *, void *, unsigned int)
	push ebp
	mov ebp, esp
	push edi
	push esi
	mov eax, [ebp+8]
	push eax
	call mylen
	add esp, 4
	add eax, [ebp+8] ; string 1 + mylen(string 1)
	mov ecx, [ebp+12] ; string 2
	mov edx, [ebp+16] ; len
	push edx
	push ecx
	push eax
	call mycpy
	add esp, 12
	pop esi 
	pop edi
	mov esp, ebp
	pop ebp
	ret
find: ; void __cdecl myfind(void *, unsigned int, void *, unsigned int);
	push ebp
	mov ebp, esp
	push edi
	push esi
	mov ecx, [ebp+12] ; size of haystack
	mov eax, [ebp+20] ; size of needle
	cmp eax, ecx
	jg .donejg ; if needle is larger than heystack return
	sub ecx, eax ; subtract haystack size from needle
	xor eax, eax
	.for:
		push eax
		push ecx
		mov eax, [ebp+20] ; needle size
		mov ecx, [ebp+16] ; needle
		mov edx, [ebp+8] ; haystack
		push eax
		push ecx
		push edx
		call mycmp
		add esp, 12
		cmp eax, 1
		jne .nada
			mov eax, [ebp+8] ; haystack
			;add eax, [ebp+20] ; haystack + needle size
			jmp .done	
		.nada:
		inc dword [ebp+8] ; inc haystack pointer
		pop ecx
		pop eax
		inc eax
		cmp eax, ecx
		jne .for
	.donejg: ; special jump to make sure eax returns null
	xor eax, eax ; return null
	.done:
	pop esi 
	pop edi
	mov esp, ebp
	pop ebp
	ret
