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

int myischarmap(char c, char *map) {
	int x, l=mylen(map);
	for(x=0; x < l; ++x)
		if(c == *(map+x))
			return 1;
	return 0;
}

void mytoupper(char *s) {
	int x, l=mylen(s);
	for(x=0; x < l; ++x)
		if(myischarmap(*(s+x), "abcdefghijklmnopqrstuvwxyz"))
			*(s+x) -= 20;
}

void mytolower(char *s) {
	int x, l=mylen(s);
	for(x=0; x < l; ++x)
		if(myischarmap(*(s+x), "ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
			*(s+x) += 20;
}

int myisdigit(char d) {
	return myischarmap(d, "0123456789");
}

int myisalpha(char a) {
	return myischarmap(a, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
}

long myatol(char *s) {
	int64_t ret=0;
	int isNeg=0, x, c, l=mylen(s);
	for(x=0; x < l && *(s+x) != 0x20; ++x)
		++s;
	if(x == l) // if it happens to be all spaces return zero
		return 0;
	if(*s == '-') {
	       isNeg = 1;
	       ++s;
	} else if (*s == '+')
		++s;
	for(c=l-x, x=0; x < c && myisdigit(*s); ++x, ++s)
		ret = 10*(ret+(*s - '0'));
	if(isNeg)
		return -ret;
	return ret;
}

uint32_t myatoi(char *s) {
	return(uint32_t)atol(s);
}
