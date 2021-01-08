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
#include <pstdint.h>
#include <coffee.c>

typedef void (*xxteap)(void *, int, void *);

/*
#define VIRTUALALLOC_LOCATION 0x75A90594
#define VIRTUALFREE_LOCATION 0x75A90CD5
*/

char *ooc_dec(char *crypted, int size, int call, char *key) {
	char *ret = pfunc.mem->alloc(size+1, sizeof(char));
	if(ret) {
		xxteap xxtea = cryptcall((void *)coffee, COFFEE_SIZE, (void *)pfunc.mem->myVirtualAlloc);
		if(xxtea) {
			mycpy(ret, crypted, size);
			xxtea((uint32_t *)ret, call, (uint32_t *)key);
			//fprintf(stdout, "Decrypted %s\n", ret);
			pfunc.mem->myVirtualFree(xxtea, 0, MEM_RELEASE);
			return ret;
		} else {
			pfunc.mem->free(ret);
			return NULL;
		}
	} else
		return NULL;
}

void ooc_free(char *b, int s) {
	myzero(b, s);
	myfree(b);
}
