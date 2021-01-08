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

unsigned char *crc_table=NULL;

#define POLY 0x04c11db7
void *gen_table() {
	if(!crc_table) {
		crc_table = myalloc(256, sizeof(char));
		if(!crc_table)
			return NULL;
		else {
			uint16_t i, j;
			uint32_t accum;
			for(i=0; i < 256; ++i) {
				accum = ((uint32_t)i << 24);
				for(j=0; j < 8; ++j) {
					if(accum & 0x80000000)
						accum = (accum << 1) ^ POLY;
					else
						accum = (accum << 1);
				}
				*(crc_table+i) = accum;
			}
		}
	} else
		return crc_table;
}

void free_table() {
	if(crc_table)
		myfree(crc_table);
}

uint32_t crc32(uint8_t *b, uint32_t size, uint32_t sum) {
	register uint32_t i, j;
	uint8_t *table = (uint8_t *)gen_table();
	if(!table) return 0;
	for(j=0; j < size; ++j) {
		i = ((uint32_t)(sum >> 24) ^ *b++) & 0xFF;
		sum = (sum << 8) ^ table[i];
	}
	return ~sum;
}
