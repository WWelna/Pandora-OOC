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

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
//#include <stdint.h>
#include <pstdint.h>


int dump_cryptkey=0;

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

#define DELTA 0x9e3779b9
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (key[(p&3)^e] ^ z)))

void btea(uint32_t *v, int n, uint32_t const key[4]) {
	uint32_t y, z, sum;
	unsigned p, rounds, e;
	if (n > 1) {          /* Coding Part */
		rounds = 6 + 52/n;
		sum = 0;
		z = v[n-1];
		do {
			sum += DELTA;
			e = (sum >> 2) & 3;
			for (p=0; p<n-1; p++) {
				y = v[p+1];
				z = v[p] += MX;
			}
			y = v[0];
			z = v[n-1] += MX;
		} while (--rounds);
	} else if (n < -1) {  /* Decoding Part */
		n = -n;
		rounds = 6 + 52/n;
		sum = rounds*DELTA;
		y = v[0];
		do {
			e = (sum >> 2) & 3;
			for (p=n-1; p>0; p--) {
				z = v[p-1];
				y = v[p] -= MX;
			}
			z = v[n-1];
			y = v[0] -= MX;
		} while ((sum -= DELTA) != 0);
    }
}

char *loadfile(char *f, uint32_t *s) {
	HANDLE hFile = CreateFile(f, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile) { // File is open!
		int totalread, size;
		char *ret;
		if(*s == 0)
			size = GetFileSize(hFile, NULL);
		else
			size = *s;
		ret = calloc(size, sizeof(char)+1);
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

char *uppercase(char *s, char *tmp) {
	int x, l=strlen(s);
	for(x=0; x < l; ++x)
		tmp[x] = toupper(s[x]);
	tmp[x] = 0;
	return tmp;
}

void usage(char *b, char *opt) {
	if(opt) {
		fprintf(stdout, "%s HAS ERROR '%s'\n", b, opt);
		exit(1);
	} else {
		fprintf(stdout, "Simple XXTEA Encryption by SanguineRose <SanguineRose@OccultusTerra.com>\n\n\t\t\t%s Command Line Flags:\n\n", b);
		fprintf(stdout, "\t/ekey:<key file> - File that has 128-bit encryption key\n");
		fprintf(stdout, "\t/ctag:<tag name> - The name for the array (required)\n");
		fprintf(stdout, "\t/size:<dwords>   - How many 32-bit / DWORDs for string encryption\n");
		fprintf(stdout, "\t/file:<file>     - Encrypt a file or assume string if not set\n");
		fprintf(stdout, "\t/ecbc:<dwords>   - Number of 32-bit / DWORDS for ECB Mode\n\n");
		fprintf(stdout, "\t/ckeyd           - Dump array of Crypt Key\n\n");
		fprintf(stdout, "\t\t\t\tExample Usage\n\n");
		fprintf(stdout, "%s /ekey:coffee.bin /ctag:cup \"I Like my coffee\"\n", b);
		fprintf(stdout, "%s /ekey:cup.bin /ctag:cup /file:topsecret.txt\n", b);
		exit(0);
	}
}

void do_dump(char *tag, int callsize, int cryptsize, int originalsize, char *b, int s, int ecb, char *k) {
	char tmp[128];
	uppercase(tag, tmp);
	if(ecb) {
		fprintf(stdout, "#define %s_ECBBLOCK %i\n", tmp, ecb);
		fprintf(stdout, "#define %s_ECBCYCLE %i\n", tmp, cryptsize/(ecb*4));
	} else
		fprintf(stdout, "#define %s_CALLSIZE -%i\n", tmp, callsize);
	fprintf(stdout, "#define %s_CRYPTSIZE %i\n", tmp, cryptsize);
	fprintf(stdout, "#define %s_ORIGSIZE %i\n", tmp, originalsize);
	dump_array_2(tag, b, s);
	if(dump_cryptkey) {
		sprintf(tmp, "%s_cryptkey", tag);
		dump_array_2(tmp, k, 16);
	}
}

int main(int argc, char **argv) {
	uint32_t *cryptkey=NULL;
	char *dyn=NULL;
	int x=0, y=0, ecb_mode=0;
	/* Options */
	char *tag=NULL, *stringtocrypt=NULL, *filetocrypt=NULL;
	int csize=0;
	/* Options */
	if(argc == 1) usage(argv[0], NULL);
	if(argv[1][0] == '/') {
		for(x=1; x < argc; ++x) {
			char *p = argv[x];
			if(argv[x][0] == '/') {
				if(strlen(argv[x])<6) usage(argv[0], "Bad Option");
				if(strncmp(argv[x], "/ekey:", 6)==0) {
					y=16;
					if(cryptkey)
						usage(argv[0], "Key Already Set");
					else
						cryptkey = (uint32_t *)loadfile(p+6, &y);
						if(y<16)
							usage(argv[0], "Encryption Key File Too Small");
				} else if(strncmp(argv[x], "/ctag:", 6)==0) {
					if(tag)
						usage(argv[0], "Tag Already Specified");
					else
						tag = p+6;
				} else if(strncmp(argv[x], "/size:", 6)==0) {
					if(csize)
						usage(argv[0], "Size Already Set");
					else
						csize = atoi(p+6);
				} else if(strncmp(argv[x], "/ckeyd", 6)==0) {
					dump_cryptkey = 1;
				} else if(strncmp(argv[x], "/file:", 6)==0) {
					if(stringtocrypt)
						usage(argv[0], "String Already Specified But File Option Set");
					else if(filetocrypt)
						usage(argv[0], "File Specified Twice");
					else
						filetocrypt = p+6;
				} else if(strncmp(argv[x], "/ecbc:", 6)==0) {
					if(ecb_mode)
						usage(argv[0], "CBC Mode Already Set");
					ecb_mode = atoi(p+6);
				} else {
					usage(argv[0], "Invalid Option Specified");
				}
			} else {
				if(filetocrypt)
					usage(argv[0], "File Specified But String Passed");
				else if(stringtocrypt)
					usage(argv[0], "String Already Specified");
				else
					stringtocrypt = p;
			}
		}
		/* Main Program */
		if(!tag) usage(argv[0], "No Tag Specified");
		if(!cryptkey) usage(argv[0], "No Encryption Key Specified");
		if(ecb_mode && stringtocrypt) usage(argv[0], "CBC Mode only supported for files");
		if(filetocrypt) { // Encrypt a file
			char *p;
			int origsize=0;
			dyn = loadfile(filetocrypt, &origsize);
			if(origsize == 0 || dyn == NULL)
				usage(argv[0], "Error Reading File");
			if(ecb_mode) {
				int ecb_bytes = ecb_mode*4;
				if(origsize%ecb_bytes)
					csize = ((origsize/ecb_bytes)*ecb_bytes)+ecb_bytes;
				else
					csize = (origsize+ecb_bytes);
				if(!(p=realloc(dyn, csize+1)))
					usage(argv[0], "Reallocation Error");
				p=dyn;
				ZeroMemory(dyn+origsize, (ecb_bytes-(origsize%ecb_bytes))+1);
				csize = csize / ecb_bytes;
				for(x=1; x <= csize; ++x, p+=ecb_bytes)
					btea((uint32_t *)p, ecb_mode, cryptkey);
				do_dump(tag, 0, csize*ecb_bytes, origsize, dyn, csize*ecb_bytes, ecb_mode, (char *)cryptkey);
				free(dyn); free(cryptkey);
				return 0;
			} else {
				if(origsize%4)
					csize = ((origsize/4)*4)+4;
				else
					csize = (origsize+4);
				if(!(p=realloc(dyn, csize+1)))
					usage(argv[0], "Reallocation Error");
				dyn=p;
				ZeroMemory(dyn+origsize, 4-(origsize%4)+1);
				csize = csize / 4;
				btea((uint32_t *)dyn, csize, cryptkey);
				do_dump(tag, csize, csize*4, origsize, dyn, csize*4, 0, (char *)cryptkey);
				free(dyn); free(cryptkey);
				return 0;
			}
		} else if(stringtocrypt) { // Encrypt a string
			int origsize=strlen(stringtocrypt);
			if(!csize) { // no size specified to calculate
				if(origsize%4)
					csize=((origsize/4)*4)+4;
				else
					csize=origsize+4; // make sure there is some kind of zero padding
				csize = csize / 4;
			}
			dyn = calloc((csize*4)+1, sizeof(char));
			if(!dyn)
				usage(argv[0], "Can't Allocate Memory Buffer For Encryption");
			CopyMemory(dyn, stringtocrypt, origsize);
			btea((uint32_t *)dyn, csize, cryptkey);
			do_dump(tag, csize, csize*4, origsize, dyn, csize*4, 0, (char *)cryptkey);
			free(dyn); free(cryptkey);
			return 0;
		} else
			usage(argv[0], "No File Or String Specified");
		/* Main Program */
	} else
		usage(argv[0], "First Argument Not An Option");
	return 0;
}

