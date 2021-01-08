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

#ifndef __ASM__STUFF__
#define __ASM__STUFF__

void __cdecl *mycpy(void *, void *, unsigned int);
void __cdecl *myset(void *, char, unsigned int);
void __cdecl *myzero(void *, unsigned int);
int __cdecl mycmp(void *, void *, unsigned int);
int __cdecl mylen(void *);
void __cdecl mycat(void *, void *, unsigned int);
void __cdecl *find(void *, unsigned int, void *, unsigned int);
void __cdecl myxor(void *, unsigned int, char);
void __cdecl myxorr(char, void *, unsigned int);
void __cdecl myinv(void *, unsigned int);
#define myfind(a, b, c, d) find(a, b, c, d)
#endif
