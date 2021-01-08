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

#ifndef __OOCDATA__
#define __OOCDATA__

struct Data_data { 
	void *buffer;
	uint32_t size;
};

struct Data_methods;
typedef struct {
	void (*new)(void *);
	struct Data_data *data;
	struct Data_methods *m;
	void (*delete)(void *);
	void (*fail)(void *, unsigned int);
	unsigned int fail_code;
	void *ptr; // pointer to the data in Data->data->buffer
} Data;

struct Data_methods {
	int (*isalloc)(Data *);
	uint32_t (*alloc)(Data *, uint32_t);
	uint32_t (*realloc)(Data *, uint32_t);
	uint32_t (*expand)(Data *, uint32_t);
	uint32_t (*retract)(Data *, uint32_t);
	uint32_t (*size)(Data *);
	char *(*data)(Data *);
	char (*gindex)(Data *, uint32_t);
	char (*sindex)(Data *, uint32_t, char);
	int (*compare)(Data *, Data *);
	int (*compare_raw)(Data *, char *, uint32_t);
	void (*set)(Data *, char);
	uint32_t (*eq)(Data *, char *, uint32_t);
	void (*zero)(Data *);
	void (*free)(Data *);
};

void Data_ctor(Data *);
int Data_m_isalloc(Data *);
uint32_t Data_m_alloc(Data *, uint32_t);
uint32_t Data_m_realloc(Data *, uint32_t);
uint32_t Data_m_expand(Data *, uint32_t);
uint32_t Data_m_retract(Data *, uint32_t);
uint32_t Data_m_size(Data *);
char *Data_m_data(Data *);
char Data_m_gindex(Data *, uint32_t);
char Data_m_sindex(Data *, uint32_t, char);
int Data_m_compare(Data *, Data *);
int Data_m_compare_raw(Data *, char *, uint32_t);
void Data_m_set(Data *, char);
uint32_t Data_m_eq(Data *, char *, uint32_t);
void Data_m_zero(Data *);
void Data_m_free(Data *);
void Data_dtor(Data *);

#endif
