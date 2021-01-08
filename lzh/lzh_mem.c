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

#include "pandora.h"
#include "lzh.h"

int lzh_read(void *d, int n, void *z) {
	compress_info *p=(compress_info *)z;
	if(p->inb!=NULL && p->in_size>0) {
		if(n <= p->in_left) {
			mycpy(d, p->inb, n);
			p->in_read += n; p->in_left -= n;
			p->inb += n;
			return n;
		} else {
			if(p->in_left<=0)
				return 0;
			else if(n>0 && p->in_left>0) {
				int x;
				mycpy(d, p->inb, p->in_left);
				x=p->in_left; p->in_read += x; p->in_left=0;
				p->inb += p->in_left;
				return x;
			} else
				return 0;
		}
	} else
		return 0;
}

int lzh_write(void *d, int n, void *z) {
	compress_info *p=(compress_info *)z;
	if(p->outb!=NULL && p->out_size>0) {
		if(n <= p->out_left) {
			mycpy(p->outb, d, n);
			p->out_written += n; p->out_left -= n;
			p->outb += n;
			return n;
		} else {
			if(p->out_left<=0)
				return 0;
			else if(n>0 && p->out_left>0) {
				int x;
				mycpy(p->outb, d, p->out_left);
				x=p->out_left; p->out_written += x; p->out_left=0;
				p->outb += p->out_left;
				return x;
			} else
				return 0;
		}
	} else
		return 0;
}

void lzh_setup(compress_info *c, char *in, int insize, char *out, int outsize) {
	c->inb = in; c->outb = out;
	c->in_left = c->in_size = insize;
	c->out_left = c->out_size = outsize;
	c->out_written = c->in_read = 0;
}
