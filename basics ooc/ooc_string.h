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

#ifndef __OOCSTRING__
#define __OOCSTRING__
#include "pandora.h"

/* String */
struct String_data {
	char *string_buffer;
	int string_size;
	int actual_size;
};

struct String_methods;
typedef struct {
	void (*new)(void *);
	struct String_data *data;
	struct String_methods *m;
	void (*delete)(void *);
	void (*fail)(void *, unsigned int);
	unsigned int fail_code;
	void *nada; // Not used
} String;

struct String_methods {
	int (*alloc)(String *, unsigned int);
	void (*set)(String *, char *, unsigned int);
	void (*setl)(String *, char *);
	void (*sets)(String *, String *);
	int (*compare)(String *, String *);
	char (*gindex)(String *, unsigned int);
	void (*sindex)(String *, unsigned int, char);
	char *(*data)(String *);
	int (*size)(String *);
	void (*zero)(String *);
	void (*append)(String *, char *);
	void *(*find)(String *, char *);
};

/* Prototypes here not really needed */
void String_ctor(String *);
int String_m_alloc(String *, unsigned int);
void String_m_set(String *, char *, unsigned int);
void String_m_setl(String *, char *); 
void String_m_sets(String *, String *);
int String_m_compare(String *, String *);
char *String_m_data(String *);
char String_m_gindex(String *, unsigned int);
void String_m_sindex(String *, unsigned int, char);
int String_m_size(String *);
void String_m_zero(String *);
void String_m_append(String *, char *);
void *String_m_find(String *, char *);
void String_dtor(String *);

/* String */
#endif
