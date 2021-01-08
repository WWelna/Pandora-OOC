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

int32_t invint(int32_t i) {
	return (0xFFFFFFFF-i)+1;
}

uint32_t compress(char *s) {
    int x=0, a=0xFF000000, b=0x00FF0000, c=0x0000FF00, d=0x000000FF;
    a &= s[0] << 24;
    b &= s[1] << 16;
    c &= s[2] << 8;
    d &= s[3];
    x = a + b + c + d;
    return x;
}

char *loadfile(char *f, uint32_t *s) {
	HANDLE hFile = CreateFile(f, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile) { // File is open!
		int totalread, size = GetFileSize(hFile, NULL);
		char *ret = myalloc(size, sizeof(char)+1);
		if(!ret) {
			CloseHandle(hFile);
			*s=0;
			return NULL;
		}
		if(ReadFile(hFile, ret, size, &totalread, NULL)!=FALSE) {
			CloseHandle(hFile);
			*s=totalread;
			return ret;
		} else {
			CloseHandle(hFile);
			*s=0;
			myfree(ret);
			return NULL;
		}
	}
}

int antivirus_cmp(char *s1, int s1_size, char *s2, int s2_size) {
	int cmp;
	if(s2_size > s1_size)
		cmp = s1_size;
	else
		cmp = s2_size;
	myxor(s1, s1_size, 0xc3);
	myxor(s2, s2_size, 0xc3);
	cmp = mycmp(s1, s2, cmp);
	myxor(s1, s1_size, 0xc3);
	myxor(s2, s2_size, 0xc3);
	return cmp;
}
