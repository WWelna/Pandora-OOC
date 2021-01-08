/* Copyright (C) 2011-2021 William Welna (wwelna@occultusterra.com)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
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

#include "fastlz.h"

#include "asm_stuff.h"
#include "ooc.h"

#include "ooc_ws2_32.h"

#include "ooc_string.h"
#include "ooc_network.h"
#include "ooc_resedit.h"
#include "ooc_skipjack.h"
#include "ooc_proxy.h"
#include "ooc_data.h"
#include "ooc_hashtable.h"

/* Pandora Boot */
extern Winsock *WSDLL;

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
/* to compare a string to mess with antivirus */
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
