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
