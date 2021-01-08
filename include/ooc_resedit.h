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

#ifndef __OOCRESEDIT__
#define __OOCRESEDIT__
#include "pandora.h"

/* ResEdit */
struct ResEdit_data {
	HANDLE handle;
};

struct ResEdit_methods;
typedef struct {
	void (*new)(void *);
	struct ResEdit_data *data;
	struct ResEdit_methods *m;
	void (*delete)(void *);
	void (*fail)(void *, unsigned int);
	unsigned int fail_code;
	void *nada; // Not used
} ResEdit;

struct ResEdit_methods {
	void (*open)(ResEdit *, char *);
	void (*update)(ResEdit *, LPCTSTR, LPCSTR, void *, uint32_t);
};

void ResEdit_ctor(ResEdit *);
void ResEdit_m_open(ResEdit *, char *);
void ResEdit_m_update(ResEdit *, LPCTSTR, LPCSTR, void *, uint32_t);
void ResEdit_dtor(ResEdit *);
/* ResEdit */
#endif
