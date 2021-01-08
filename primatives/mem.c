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

void *myalloc(unsigned int objects, unsigned int size) {
	HANDLE hHeap = pfunc.mem->myGetProcessHeap();
	char *p=NULL;
	if(pfunc.mem->myHeapLock(hHeap)) {
		uint32_t s = objects*size;
		p = pfunc.mem->myHeapAlloc(hHeap, 0, s);
		if(p)
			myzero(p, s);
		pfunc.mem->myHeapUnlock(hHeap);
	}
	return p;
}

void *myrealloc(void *p, unsigned int objects, unsigned int size) {
	HANDLE hHeap = pfunc.mem->myGetProcessHeap();
	char *r=NULL;
	if(p) {
		if(pfunc.mem->myHeapLock(hHeap)) {
			uint32_t s = objects*size;
			uint32_t f = pfunc.mem->myHeapSize(hHeap, 0, p); // get the initial size before reallocation call
			p = pfunc.mem->myHeapReAlloc(hHeap, 0, p, s);
			if(s > f) { // if expanded zero the new allocated memory
				myzero(p+f, s-f);
			}
			pfunc.mem->myHeapUnlock(hHeap);
		}
	} else {
		r = myalloc(objects, size);
	}
	return r;
}

void *myrealloc2(void *p, unsigned int size) {
	return myrealloc(p, 1, size);
}

void myfree(void *p) {
	HANDLE hHeap = pfunc.mem->myGetProcessHeap();
	if(pfunc.mem->myHeapLock(hHeap)) {
		pfunc.mem->myHeapFree(hHeap, 0, p);
		pfunc.mem->myHeapUnlock(hHeap);
	}
}

uint32_t mysize(void *p) {
	HANDLE hHeap = pfunc.mem->myGetProcessHeap();
	if(pfunc.mem->myHeapLock(hHeap)) {
		uint32_t s = pfunc.mem->myHeapSize(hHeap, 0, p);
		pfunc.mem->myHeapUnlock(hHeap);
		return s;
	}
}
