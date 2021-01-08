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

#ifndef __OOC__
#define __OOC__
#include "pandora.h"

/* Main Object Creation, Destruction, and Exception Handling */
#define New(a) new_ctor((gen_ctor)a)
#define Delete(a) delete_dtor((generic_object *)a)

typedef (*gen_ctor)(void *);
typedef (*gen_dtor)(void *);
typedef (*gen_fail)(void *, unsigned int);

typedef struct {
	void *ctor;
	void *data;
	void *methods;
	void *dtor;
	void *fail;
	unsigned int fail_code;
	void *add; // Special additional info only destroyed on Delete()
} generic_object;

void *new_ctor(gen_ctor);
void gen_exception(generic_object *, unsigned int);
void gen_exception_noop(generic_object *, unsigned int);
void delete_dtor(generic_object *o);
/* Main Object Creation, Destruction, and Exception Handling */

#endif
