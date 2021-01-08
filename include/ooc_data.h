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
