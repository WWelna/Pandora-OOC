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

const static struct {
	void (*open)(ResEdit *, char *);
	void (*update)(ResEdit *, LPCTSTR, LPCSTR, void *, uint32_t);
} ResEdit_methods_static = { &ResEdit_m_open, &ResEdit_m_update };

void ResEdit_ctor(ResEdit *r) {
	r->new = (void *)&ResEdit_ctor;
	r->delete = (void *)&ResEdit_dtor;
	r->m = myalloc(1, sizeof(struct ResEdit_methods));
	if(!r->m)
		r->fail((void *)r, 1);
	else {
		mycpy(r->m, (void *)&ResEdit_methods_static, sizeof(struct ResEdit_methods)); // Set the methods to the generic ones
		r->data = myalloc(1, sizeof(struct ResEdit_data));
		if(!r->m)
			r->fail((void *)r, 2);
	}
}

void ResEdit_m_open(ResEdit *r, char *file) {
	if(r->fail_code == 0 && !r->data->handle)
		if(!(r->data->handle=BeginUpdateResource(file, FALSE)))
			r->fail(r, 100);
}

void ResEdit_m_update(ResEdit *r, LPCTSTR type, LPCSTR location, void *data, uint32_t size) {
	if(r->fail_code == 0)
		if(!UpdateResource(r->data->handle, type, location, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), data, size))
			r->fail(r, 101);
}

void ResEdit_dtor(ResEdit *r) {
	if(r->data && r->data->handle)
		EndUpdateResource(r->data->handle, FALSE);
}
