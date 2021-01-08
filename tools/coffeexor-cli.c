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

#include <windows.h>
#include <stdio.h>

void __cdecl *coffeexor(void *, int);

void dump_array_2(char *name, char *data, int size) {
	int x=0, y=0; unsigned char c;
	fprintf(stdout, "char %s[] = {\n\t", name);
	for(x=0; x < size; ++x) {
		c=data[x];
		if(y == 8) {
			fprintf(stdout, "\n\t");
			y=0;
		}
		if(x == size-1)
			fprintf(stdout, "0x%02X\n", c);
		else
			fprintf(stdout, "0x%02X, ", c);
		++y;
	}
	fprintf(stdout, "};\n");
}

char *loadfile(char *f, int *s) {
	HANDLE hFile = CreateFile(f, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile) { // File is open!
		int totalread, size;
		char *ret;
		if(*s == 0)
			size = GetFileSize(hFile, NULL);
		else
			size = *s;
		ret = (char *)calloc(size, sizeof(char)+1);
		if(ret==NULL) {
			CloseHandle(hFile);
			*s=0;
			return NULL;
		}
		if(ReadFile(hFile, ret, size, &totalread, NULL)!=FALSE) {
			CloseHandle(hFile);
			*s=totalread;
			return ret;
		}
		else {
			CloseHandle(hFile);
			*s=0;
			return NULL;
		}
	} else
		return NULL;
}

int main(int argc, char **argv) {
	char *f;
	int s;
	if(argc != 2) exit(1);
	f=loadfile(argv[1], &s);
	if(f == NULL || s == 0) exit(2);
	fprintf(stdout, "#define COFFEE_SIZE %i\n", s);
	dump_array_2("coffee", coffeexor(f, s), s);
	return 0;
}
