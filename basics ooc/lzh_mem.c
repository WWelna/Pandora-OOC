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
