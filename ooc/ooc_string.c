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

/* String Object */
static const struct {
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
} String_methods_static = { &String_m_alloc, &String_m_set, &String_m_setl, &String_m_sets, &String_m_compare, &String_m_gindex, &String_m_sindex, &String_m_data, &String_m_size, &String_m_zero, &String_m_append, &String_m_find };

int String_m_alloc(String *s, unsigned int size) { // Allocate to nearest 1k value and zero if initial value 
	size += 1;
	if(s->data->string_buffer) {
		if(size > s->data->actual_size) {
			unsigned int memcalc = (size/1024)*1024;
			if(size%1024)
				memcalc += 1024;
			char *tmp = myrealloc2(s->data->string_buffer, memcalc);
			if(tmp) {
				s->data->string_buffer = tmp;
				s->data->actual_size = memcalc;
				return 1;
			} else {
				s->fail((void *)s, 3); // Fail Allocation #3
				return 0;
			}
		} // If size is less than actual size keep it rather then shrinking the buffer
		return 1;
	} else {
		unsigned int memcalc = (size/1024)*1024;
		if(size%1024)
			memcalc += 1024;
		if((s->data->string_buffer=myalloc(memcalc, 1))) {
			s->data->actual_size = memcalc;
		} else {
			s->data->actual_size = memcalc;
		}
		//s->m->zero(s); // clear the string
		return 1;
	}
	return 0; // never reached
}

void String_m_set(String *s, char *data, unsigned int size) {
	if(s->m->alloc(s, size)) { // do required memory allocation
		myset(s->data->string_buffer, 0, s->data->actual_size); // Zero it all
		mycpy(s->data->string_buffer, data, size);
		s->data->string_size = size;
	}
}

void String_m_setl(String *s, char *data) {
	s->m->set(s, data, mylen(data));
}

void String_m_sets(String *s, String *s2) {
	s->m->set(s, s2->data->string_buffer, s2->data->string_size);
}

int String_m_compare(String *s, String *s1) {
	if(s->data->string_size == s1->data->string_size) { // Size matches
		if(mycmp(s->data->string_buffer, s1->data->string_buffer, s->data->string_size))
			return 1; // is identical
		else
			return 0;
	} else
		return 0;
}

char String_m_gindex(String *s, unsigned int index) {
	if((index+1) < s->data->actual_size) {
		return s->data->string_buffer[index]; // return the index
	} else
		return '\0'; // return 0 if out of bounds
}

void String_m_sindex(String *s, unsigned int index, char c) {
	if(s->data->string_buffer && ((index+1) < s->m->size(s))) { // Within Bounds
		*(s->data->string_buffer+index) = c;
	} else { // OOBs so expand buffer
		if(s->m->alloc(s, (index+1))) { // Make sure alloc is successfull w/out exceptions 
			//myset(s->data->string_buffer+s->data->string_size, 0, (s->data->actual_size-s->data->string_size)); // Zero rest of string
			*(s->data->string_buffer+index) = c;
		}
	}
}

char *String_m_data(String *s) {
	return s->data->string_buffer;
}


int String_m_size(String *s) {
	if(s->data->string_buffer) // if not null re-calculated string size
		s->data->string_size = mylen(s->data->string_buffer);
	return s->data->string_size;
}

void String_m_zero(String *s) {
	if(s->data->string_buffer)
		myzero(s->data->string_buffer, s->data->actual_size);
}

void String_m_append(String *s, char *d) {
	if(d) {
		if(s->data->string_buffer == NULL) { // if NULL just set the string
			s->m->setl(s, d);
		} else {
			int len=mylen(d);
			if(s->m->alloc(s, len+s->data->string_size+1)) { // make sure there is enough space
				mycat(s->data->string_buffer, d, len);
			}
		}
	}
}

void *String_m_find(String *s, char *b) {
	if(s->data->string_buffer && b)
		return find(s->m->data(s), s->m->size(s), b, mylen(b));
	else
		return NULL;
}

void String_ctor(String *s) {
	s->new = (void *)&String_ctor;
	s->delete = (void *)&String_dtor;
	s->m = myalloc(1, sizeof(struct String_methods));
	if(!s->m)
		s->fail((void *)s, 1);
	else {
		mycpy(s->m, (void *)&String_methods_static, sizeof(struct String_methods)); // Set the methods to the generic ones
		s->data = myalloc(1, sizeof(struct String_data));
		if(!s->m)
			s->fail((void *)s, 2);
	}
}

void String_dtor(String *s) {
	if(s->data && s->data->string_buffer)
		myfree(s->data->string_buffer);
}
/* String Object */
