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

uint64_t fnv1a(char *, uint32_t);

struct hashtable_list_struct {
	uint64_t hash;
	Data *hash_key;
	Data *hash_value;
	void *n;
};

static const struct {
	void (*push_sd)(Hashtable *, char *, Data *);
	void (*push_dd)(Hashtable *, Data *, Data *);
	void (*push_ss)(Hashtable *, char *, char *);
	void (*delete_s)(Hashtable *, char *);
	void (*delete_d)(Hashtable *, Data *);
	char *(*find_ss)(Hashtable *, char *);
	Data *(*find_ds)(Hashtable *, char *);
	Data *(*find_dd)(Hashtable *, Data *);
} Hashtable_methods_static = { &Hashtable_m_push_sd, &Hashtable_m_push_dd, &Hashtable_m_push_ss, &Hashtable_m_delete_s, &Hashtable_m_delete_d, &Hashtable_m_find_ss, &Hashtable_m_find_ds, &Hashtable_m_find_dd };

void Hashtable_p_push(Hashtable *, Data *, Data *);

void Hashtable_ctor(Hashtable *h) {
	h->new = (void *)&Hashtable_ctor;
	h->delete = (void *)&Hashtable_dtor;
	h->m = myalloc(1, sizeof(struct Hashtable_methods));
	if(!h->m)
		h->fail(h, 1);
	else {
		mycpy(h->m, (void *)&Hashtable_methods_static, sizeof(struct Hashtable_methods));
		h->data = myalloc(1, sizeof(struct Hashtable_data));
		if(!h->data)
			h->fail(h, 2);
	}
}


void Hashtable_m_push_sd(Hashtable *h, char *key, Data *value) {
	if(key && value) {
		if(mylen(key)>0 && value->m->size(value) > 0) {
			Data *k = New(Data_ctor);
			Data *v = New(Data_ctor);
			k->m->eq(k, key, mylen(key));
			v->m->eq(v, value->m->data(value), value->m->size(value));
			Hashtable_p_push(h, k, v);
		}
	}	
}

void Hashtable_m_push_dd(Hashtable *h, Data *key, Data *value) {
	if(key && value) {
		if(key->m->size(key)>0 && value->m->size(value)>0) {
			Data *k = New(Data_ctor);
			Data *v = New(Data_ctor);
			k->m->eq(k, key->m->data(key), key->m->size(key));
			v->m->eq(v, value->m->data(value), value->m->size(value));
			Hashtable_p_push(h, k, v);
		}
	}	
}

void Hashtable_m_push_ss(Hashtable *h, char *key, char *value) {
	if(key && value) {
		if(mylen(key)>0 && mylen(value)>0) {
			Data *k = New(Data_ctor);
			Data *v = New(Data_ctor);
			k->m->eq(k, key, mylen(key));
			v->m->eq(v, value, mylen(value));
			Hashtable_p_push(h, k, v);
		}
	}	
}

void Hashtable_p_push(Hashtable *h, Data *key, Data *value) { // Actually does the work
	if(key && value) {
		if(key->fail_code != 0 && value->fail_code != 0) { // make sure data objects are ok
			Delete(key); Delete(value);
			h->fail(h, 10);
		} else
			if(h->data->hashes) { // Append to end
				struct hashtable_list_struct *p, *e = myalloc(1, sizeof(struct hashtable_list_struct));
				if(e) {
					e->hash = fnv1a(key->m->data(key), key->m->size(key));
					e->hash_key = key;
					e->hash_value = value;
					e->n = NULL;
					for(p=h->data->hashes; p->n != NULL; )
						p=(struct hashtable_list_struct *)p->n;
					p->n = (void *)e;
				} else
					h->fail(h, 11);
			} else { // initial hash
				struct hashtable_list_struct *e = myalloc(1, sizeof(struct hashtable_list_struct));
				if(e) {
					e->hash = fnv1a(key->m->data(key), key->m->size(key));
					e->hash_key = key;
					e->hash_value = value;
					e->n = NULL;
					h->data->hashes = e;
				} else
					h->fail(h, 11);
			}
	}	
}

void Hashtable_m_delete_s(Hashtable *h, char *key) {
}

void Hashtable_m_delete_d(Hashtable *h, Data *key) {
}

char *Hashtable_m_find_ss(Hashtable *h, char *key) {
}

Data *Hashtable_m_find_ds(Hashtable *h, char *key) {
}

Data *Hashtable_m_find_dd(Hashtable *h, Data *key) {
}

void Hashtable_dtor(Hashtable *h) {
}
