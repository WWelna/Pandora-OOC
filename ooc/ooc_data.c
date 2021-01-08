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
