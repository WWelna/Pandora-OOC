/*
* Copyright (C) 2016 William H. Welna All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY William H. Welna ''AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL William H. Welna BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* -- Pandora Source Code --
 *
 * None of this source code released will compile without full understanding of
 * the code as well as some work. This is only a fraction of the code and these
 * select parts are for informational purposes only. This for teaching and showing
 * how to code in a OOC style to trip up disassemblers and antivirus software.
 *
 *
*/

#ifndef __PANDORA__H__
#define __PANDORA__H__

#define _WIN32_WINNT 0x501

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Winbase.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pstdint.h"

#include "asm_stuff.h"
#include "ooc.h"

#include "ooc_string.h"
#include "ooc_data.h"

struct pmem_export {
	void *(*alloc)(unsigned int, unsigned int);
	void *(*realloc)(void *, unsigned int, unsigned int);
	void *(*realloc2)(void *, unsigned int);
	void (*free)(void *);
	uint32_t (*size)(void *);
	void *(*myVirtualAlloc)(void *, int, int, int);
	int (*myVirtualFree)(void *, int, int);
	HANDLE (*myGetProcessHeap)(void);
	void *(*myHeapAlloc)(HANDLE, unsigned int, unsigned int);
	void *(*myHeapReAlloc)(HANDLE, unsigned int, void *, unsigned int);
	uint32_t (*myHeapSize)(HANDLE, unsigned int, void *);
	int (*myHeapFree)(void *, int, void *);
	int (*myHeapLock)(HANDLE);
	int (*myHeapUnlock)(HANDLE);
};

struct pfunc_export {
	struct pmem_export *mem;
};

extern struct pfunc_export pfunc;

int pandora_network();
int pandora_boot();
/* Pandora Boot */

/* For string crypting */
void __cdecl *cryptcall(void *, int, void *);
char *ooc_dec(char *, int, int, char *);
void ooc_free(char *, int);
/* Invert Int */
int32_t invint(int32_t i);
/* read 4 chars (8 bits) and return a 32-bit int */
uint32_t compress(char *s);
/* Load a file and set size pointed to, return allocated buffer */
char *loadfile(char *, uint32_t *);
/* to compare a string to fuck with antivirus */
int antivirus_cmp(char *, int, char *, int);

/* Checksums */
uint32_t Adler32(uint8_t *, uint32_t, uint32_t);
uint32_t crc32(uint8_t *, uint32_t, uint32_t);
void *gen_table(); // for crc32
void free_table(); // for crc32

/* Memory Fuctions */
void *myalloc(unsigned int, unsigned int);
void *myrealloc(void *, unsigned int, unsigned int);
void *myrealloc2(void *, unsigned int);
void myfree(void *);
uint32_t mysize(void *);

/* Some String Stuff */
int myischarmap(char, char *);
void mytoupper(char *);
void mytolower(char *);
int myisdigit(char);
int myisalpha(char);
long myatol(char *);
uint32_t myatoi(char *);

#endif
