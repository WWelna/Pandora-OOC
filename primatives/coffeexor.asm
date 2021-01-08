; Coffee
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
global coffeexor

coffeexor: ; void __cdecl *coffeexor(void *, int);
	push ebp
	mov ebp, esp
	push edi
	push esi
	mov ecx, [ebp+12];
	mov edi, [ebp+8];
	mov esi, edi
	push esi ; push pointer buffer
	.loop:
		lodsb
		xor al, cl
		xor al, 0xee
		xor al, 0xfe
		xor al, 0x0f
		xor al, cl
		xor al, 0xc0
		xor al, 0x37
		xor al, 0x13
		xor al, cl
		stosb
		loop .loop
	pop eax ; buffer pointer
	pop esi
	pop edi
	mov esp, ebp
	pop ebp
	ret
