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

/* Global Function Tables */
static struct pmem_export pmem = { &myalloc, &myrealloc, &myrealloc2, &myfree, &mysize, &VirtualAlloc, &VirtualFree, &GetProcessHeap, &HeapAlloc, &HeapReAlloc, &HeapSize, &HeapFree, &HeapLock, &HeapUnlock };
static struct pfunc_export pfunc = { &pmem };
/* Global Function Tables */

/* Global Objects */
Winsock *WSDLL=NULL;
/* Global Objects */

int pandora_network() {
	if(!WSDLL) {
		WSDLL = New(Winsock_ctor);
		WSDLL->m->load(WSDLL); // Load Winsocks
		if(WSDLL->fail_code != 0 && !WSDLL->m->isloaded(WSDLL)) { // Failed loading so free
			Delete(WSDLL);
			WSDLL = NULL;
			return 0;
		}
	} else
		return 1; // already loaded
}

int pandora_boot() {
	if(!pandora_network()) return 0;
	return 1;
}
