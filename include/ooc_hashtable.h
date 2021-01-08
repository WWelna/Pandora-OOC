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

#ifndef __OOCHASHTABLE__
#define __OOCHASHTABLE__

struct hashtable_list_struct;
struct Hashtable_data {
	struct hashtable_list_struct *hashes;
};

struct Hashtable_methods;
typedef struct {
	void (*new)(void *);
	struct Hashtable_data *data;
	struct Hashtable_methods *m;
	void (*delete)(void *);
	void (*fail)(void *, unsigned int);
	unsigned int fail_code;
	void *shit;
} Hashtable;

struct Hashtable_methods {
	void (*push_sd)(Hashtable *, char *, Data *);
	void (*push_dd)(Hashtable *, Data *, Data *);
	void (*push_ss)(Hashtable *, char *, char *);
	void (*delete_s)(Hashtable *, char *);
	void (*delete_d)(Hashtable *, Data *);
	char *(*find_ss)(Hashtable *, char *);
	Data *(*find_ds)(Hashtable *, char *);
	Data *(*find_dd)(Hashtable *, Data *);
};

void Hashtable_ctor(Hashtable *);
void Hashtable_m_push_sd(Hashtable *, char *, Data *);
void Hashtable_m_push_dd(Hashtable *, Data *, Data *);
void Hashtable_m_push_ss(Hashtable *, char *, char *);
void Hashtable_m_delete_s(Hashtable *, char *);
void Hashtable_m_delete_d(Hashtable *, Data *);
char *Hashtable_m_find_ss(Hashtable *, char *);
Data *Hashtable_m_find_ds(Hashtable *, char *);
Data *Hashtable_m_find_dd(Hashtable *, Data *);
void Hashtable_dtor(Hashtable *);

#endif
