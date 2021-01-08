; Decompiled XXTEA made compilable again
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

use32

xxtea_asm:
	push ebp
	mov ebp,esp
	sub esp,0x38
	mov eax,[ebp+0xc]
	cmp eax,byte +0x1
	jng .loc_18C
	mov eax,0x34
	mov ecx,[ebp+0xc]
	cdq
	idiv ecx
	add eax,byte +0x6
	mov [ebp-0x18],eax
	mov eax,0x0
	mov [ebp-0x14],eax
	mov eax,[ebp+0xc]
	sub eax,byte +0x1
	shl eax,0x2
	mov ecx,[ebp+0x8]
	add ecx,eax
	mov eax,[ecx]
	mov [ebp-0x8],eax
.loc_42:
	mov eax,[ebp-0x14]
	add eax,0x9e3779b9
	mov [ebp-0x14],eax
	mov eax,[ebp-0x14]
	shr eax,0x2
	and eax,byte +0x3
	mov [ebp-0xc],eax
	mov eax,0x0
	mov [ebp-0x10],eax
.loc_62:
	mov eax,[ebp+0xc]
	sub eax,byte +0x1
	mov ecx,[ebp-0x10]
	cmp ecx,eax
	jnc  .loc_104
	jmp  .loc_85
.loc_78:
	mov eax,[ebp-0x10]
	mov ecx,eax
	add eax,byte +0x1
	mov [ebp-0x10],eax
	jmp  .loc_62
.loc_85:
	mov eax,[ebp-0x10]
	add eax,byte +0x1
	shl eax,0x2
	mov ecx,[ebp+0x8]
	add ecx,eax
	mov eax,[ecx]
	mov [ebp-0x4],eax
	mov eax,[ebp-0x10]
	shl eax,0x2
	mov ecx,[ebp+0x8]
	add ecx,eax
	mov eax,[ebp-0x8]
	shr eax,0x5
	mov edx,[ebp-0x4]
	shl edx,0x2
	xor eax,edx
	mov edx,[ebp-0x4]
	shr edx,0x3
	mov [ebp-0x1c],ecx
	mov ecx,[ebp-0x8]
	shl ecx,0x4
	xor edx,ecx
	add eax,edx
	mov ecx,[ebp-0x14]
	mov edx,[ebp-0x4]
	xor ecx,edx
	mov edx,[ebp-0x10]
	and edx,byte +0x3
	mov [ebp-0x20],eax
	mov eax,[ebp-0xc]
	xor edx,eax
	shl edx,0x2
	mov eax,[ebp+0x10]
	add eax,edx
	mov edx,[eax]
	mov eax,[ebp-0x8]
	xor edx,eax
	add ecx,edx
	mov eax,[ebp-0x20]
	xor eax,ecx
	mov ecx,[ebp-0x1c]
	mov ecx,[ecx]
	add ecx,eax
	mov eax,[ebp-0x1c]
	mov [eax],ecx
	mov [ebp-0x8],ecx
	jmp  .loc_78
.loc_104:
	mov eax,[ebp+0x8]
	mov ecx,[eax]
	mov [ebp-0x4],ecx
	mov eax,[ebp+0xc]
	sub eax,byte +0x1
	shl eax,0x2
	mov ecx,[ebp+0x8]
	add ecx,eax
	mov eax,[ebp-0x8]
	shr eax,0x5
	mov edx,[ebp-0x4]
	shl edx,0x2
	xor eax,edx
	mov edx,[ebp-0x4]
	shr edx,0x3
	mov [ebp-0x24],ecx
	mov ecx,[ebp-0x8]
	shl ecx,0x4
	xor edx,ecx
	add eax,edx
	mov ecx,[ebp-0x14]
	mov edx,[ebp-0x4]
	xor ecx,edx
	mov edx,[ebp-0x10]
	and edx,byte +0x3
	mov [ebp-0x28],eax
	mov eax,[ebp-0xc]
	xor edx,eax
	shl edx,0x2
	mov eax,[ebp+0x10]
	add eax,edx
	mov edx,[eax]
	mov eax,[ebp-0x8]
	xor edx,eax
	add ecx,edx
	mov eax,[ebp-0x28]
	xor eax,ecx
	mov ecx,[ebp-0x24]
	mov ecx,[ecx]
	add ecx,eax
	mov eax,[ebp-0x24]
	mov [eax],ecx
	mov [ebp-0x8],ecx
	mov eax,[ebp-0x18]
	add eax,byte -0x1
	mov [ebp-0x18],eax
	test eax,eax
	jnz  .loc_42
	jmp  .loc_305
.loc_18C:
	mov eax,[ebp+0xc]
	cmp eax,byte -0x1
	jnl  .loc_305
	mov eax,0x0
	mov ecx,[ebp+0xc]
	sub eax,ecx
	mov [ebp+0xc],eax
	mov eax,0x34
	mov ecx,[ebp+0xc]
	cdq
	idiv ecx
	add eax,byte +0x6
	mov [ebp-0x18],eax
	mov eax,[ebp-0x18]
	mov ecx,0x9e3779b9
	imul eax,ecx
	mov [ebp-0x14],eax
	mov eax,[ebp+0x8]
	mov ecx,[eax]
	mov [ebp-0x4],ecx
.loc_1CC:
	mov eax,[ebp-0x14]
	shr eax,0x2
	and eax,byte +0x3
	mov [ebp-0xc],eax
	mov eax,[ebp+0xc]
	sub eax,byte +0x1
	mov [ebp-0x10],eax
.loc_1E1:
	mov eax,[ebp-0x10]
	cmp eax,byte +0x0
	jna  .loc_27E
	jmp  .loc_1FF
.loc_1F2:
	mov eax,[ebp-0x10]
	mov ecx,eax
	add eax,byte -0x1
	mov [ebp-0x10],eax
	jmp  .loc_1E1
.loc_1FF:
	mov eax,[ebp-0x10]
	sub eax,byte +0x1
	shl eax,0x2
	mov ecx,[ebp+0x8]
	add ecx,eax
	mov eax,[ecx]
	mov [ebp-0x8],eax
	mov eax,[ebp-0x10]
	shl eax,0x2
	mov ecx,[ebp+0x8]
	add ecx,eax
	mov eax,[ebp-0x8]
	shr eax,0x5
	mov edx,[ebp-0x4]
	shl edx,0x2
	xor eax,edx
	mov edx,[ebp-0x4]
	shr edx,0x3
	mov [ebp-0x2c],ecx
	mov ecx,[ebp-0x8]
	shl ecx,0x4
	xor edx,ecx
	add eax,edx
	mov ecx,[ebp-0x14]
	mov edx,[ebp-0x4]
	xor ecx,edx
	mov edx,[ebp-0x10]
	and edx,byte +0x3
	mov [ebp-0x30],eax
	mov eax,[ebp-0xc]
	xor edx,eax
	shl edx,0x2
	mov eax,[ebp+0x10]
	add eax,edx
	mov edx,[eax]
	mov eax,[ebp-0x8]
	xor edx,eax
	add ecx,edx
	mov eax,[ebp-0x30]
	xor eax,ecx
	mov ecx,[ebp-0x2c]
	mov ecx,[ecx]
	sub ecx,eax
	mov eax,[ebp-0x2c]
	mov [eax],ecx
	mov [ebp-0x4],ecx
	jmp  .loc_1F2
.loc_27E:
	mov eax,[ebp+0xc]
	sub eax,byte +0x1
	shl eax,0x2
	mov ecx,[ebp+0x8]
	add ecx,eax
	mov eax,[ecx]
	mov [ebp-0x8],eax
	mov eax,[ebp+0x8]
	mov ecx,[ebp-0x8]
	shr ecx,0x5
	mov edx,[ebp-0x4]
	shl edx,0x2
	xor ecx,edx
	mov edx,[ebp-0x4]
	shr edx,0x3
	mov [ebp-0x34],eax
	mov eax,[ebp-0x8]
	shl eax,0x4
	xor edx,eax
	add ecx,edx
	mov eax,[ebp-0x14]
	mov edx,[ebp-0x4]
	xor eax,edx
	mov edx,[ebp-0x10]
	and edx,byte +0x3
	mov [ebp-0x38],ecx
	mov ecx,[ebp-0xc]
	xor edx,ecx
	shl edx,0x2
	mov ecx,[ebp+0x10]
	add ecx,edx
	mov edx,[ecx]
	mov ecx,[ebp-0x8]
	xor edx,ecx
	add eax,edx
	mov ecx,[ebp-0x38]
	xor ecx,eax
	mov eax,[ebp-0x34]
	mov eax,[eax]
	sub eax,ecx
	mov ecx,[ebp-0x34]
	mov [ecx],eax
	mov [ebp-0x4],eax
	mov eax,[ebp-0x14]
	sub eax,0x9e3779b9
	mov [ebp-0x14],eax
	cmp eax,byte +0x0
	jnz  .loc_1CC
.loc_305:
	leave
	retn
