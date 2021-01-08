; Crypt Call
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
global cryptcall

cryptcall: ; void __cdecl *cryptcall(void *, int, void *);
	push ebp
	mov ebp, esp
	push edi
	push esi
	xor eax, eax
	mov esi, [ebp+8] ; used below
	cmp esi, 0
	je .naughty
	mov ecx, [ebp+16]
	cmp ecx, 0
	je .naughty ; make sure allocate pointer is passed
	push dword 0x40
	push dword 0x1000
	push dword [ebp+12]
	xor eax, eax ; Null Base Address
	push eax
	call ecx
	add esp, 16
	cmp eax, 0
	je .naughty ; if null pointer returned exit and return null
	mov edi, eax
	mov edx, edi
	mov esi, [ebp+8]
	mov ecx, [ebp+12]
	cld
	rep movsb ; Copy buffer to newly allocated buffer by VirtualAlloc
	mov edi, edx
	mov esi, edx
	mov ecx, [ebp+12]
	.loop:
		lodsb
		xor al, cl
		xor al, 0x13
		xor al, 0x37
		xor al, 0xc0 ; mmmm Coffee
		xor al, cl
		xor al, 0x0f
		xor al, 0xfe
		xor al, 0xee
		xor al, cl
		stosb
		loop .loop
	mov eax, edx
	.naughty:
	pop esi
	pop edi
	leave
	ret
