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
