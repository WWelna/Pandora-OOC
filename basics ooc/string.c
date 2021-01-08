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
