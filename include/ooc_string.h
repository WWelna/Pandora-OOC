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
