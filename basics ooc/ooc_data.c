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

#include <pandora.h>

static const struct {
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
} Data_methods_static = { &Data_m_isalloc, &Data_m_alloc, &Data_m_realloc, &Data_m_expand, &Data_m_retract, &Data_m_size, &Data_m_data, &Data_m_gindex, &Data_m_sindex, &Data_m_compare, &Data_m_compare_raw, &Data_m_set, &Data_m_eq, &Data_m_zero, &Data_m_free };

void Data_ctor(Data *d) {
	d->new = (void *)&Data_ctor;
	d->delete = (void *)&Data_dtor;
	d->m = myalloc(1, sizeof(struct Data_methods));
	if(!d->m)
		d->fail(d, 1);
	else {
		mycpy(d->m, (void *)&Data_methods_static, sizeof(struct Data_methods));
		d->data = myalloc(1, sizeof(struct Data_data));
		if(!d->data)
			d->fail(d, 2);
	}
}

int Data_m_isalloc(Data *d) {
	if(d->data->buffer == NULL)
		return 0;
	else if(d->data->size == 0)
		return 0;
	return 1;
}

uint32_t Data_m_alloc(Data *d, uint32_t size) {
	if(size == 0) return 0;
	if(d->data->buffer) { // if already allocated try to realloc
		return d->m->realloc(d, size);
	} else { // initial allocation
		void *p = myalloc(size, 1);
		if(p) { // success
			d->data->buffer = d->ptr = p;
			d->data->size = size;
			return size;
		} else { // fail
			d->data->buffer = d->ptr = NULL;
			d->data->size = 0;
			d->fail(d, 10);
			return 0;
		}
	}
	return 0;
}

uint32_t Data_m_realloc(Data *d, uint32_t size) {
	if(size == 0 || size == d->data->size) return 0;
	if(d->data->buffer) {
		void *p = myrealloc2(d->data->buffer, size);
		if(p) { // success
			d->data->buffer = d->ptr = p;
			d->data->size = size;
			return size;
		} else { // fail
			d->data->buffer = d->ptr = NULL;
			d->data->size = 0;
			d->fail(d, 11);
			return 0;
		}
	} else
		return d->m->alloc(d, size);
}

uint32_t Data_m_expand(Data *d, uint32_t size) {
	if(size == 0) return 0;
	if(d->data->buffer) {
		return d->m->realloc(d, d->data->size + size);
	} else
		return d->m->alloc(d, size);
}

uint32_t Data_m_retract(Data *d, uint32_t size) {
	if(size == 0) return 0;
	if(d->data->buffer) {
		if(size >= d->data->size) {
			d->m->free(d);
			return 0;
		} else 
			return d->m->realloc(d, d->data->size - size);
	} else {
		d->fail(d, 12); // retracting nothing?!??!
		return 0;
	}
}

uint32_t Data_m_size(Data *d) {
	return d->data->size;
}

char *Data_m_data(Data *d) {
	return d->data->buffer;
}

char Data_m_gindex(Data *d, uint32_t i) {
	if((i+1) < d->data->size)
		return d->data->buffer[i];
	else
		return '\0';
}

char Data_m_sindex(Data *d, uint32_t i, char c) {
	if((i+1) < d->data->size) {
		d->data->buffer[i] = c;
		return d->data->buffer[i];
	} else
		return '\0';
}

int Data_m_compare(Data *d, Data *d2) {
	if(d2 && d->data->buffer && d2->data->buffer)
		return d->m->compare_raw(d, d2->data->buffer, d2->data->size);
}

int Data_m_compare_raw(Data *d, char *b, uint32_t size) {
	if(d->data->buffer) {
		uint32_t cmp_size = size;
		if(cmp_size > d->data->size)
			cmp_size = d->data->size;
		return mycmp(d->data->buffer, b, cmp_size);
	}
	return 0;
}

void Data_m_set(Data *d, char c) {
	if(d->data->buffer)
		myset(d->data->buffer, c, d->data->size);
}

uint32_t Data_m_eq(Data *d, char *b, uint32_t size) {
	uint32_t s = d->m->alloc(d, size);
	if(s)
		mycpy(d->data->buffer, b, size);
	return s;
}

void Data_m_zero(Data *d) {
	if(d->data->buffer)
		myzero(d->data->buffer, d->data->size);
}

void Data_m_free(Data *d) {
	if(d->data->buffer) {
		myfree(d->data->buffer);
		d->data->buffer = d->ptr = NULL;
		d->data->size = 0;
	}
}

void Data_dtor(Data *d) {
	d->m->free(d);
}
