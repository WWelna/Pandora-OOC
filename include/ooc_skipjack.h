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

#ifndef __OOCSKIPJACK__
#define __OOCSKIPJACK__
#include "pandora.h"

/* Crypt_Skipjack */
struct Crypt_Skipjack_data {
	uint8_t block[8]; // temp buffer
	uint8_t que_isset;
	uint8_t que[8]; // For finalization / padding operations
	uint8_t context[10][256]; // Teh Context
};

struct Crypt_Skipjack_stats {
	uint64_t total_processed; // Total amount encrypted & decrypted, reset on reset();
};

struct Crypt_Skipjack_methods;
typedef struct {
	void (*new)(void *);
	struct Crypt_Skipjack_data *data;
	struct Crypt_Skipjack_methods *m;
	void (*delete)(void *);
	void (*fail)(void *, unsigned int);
	unsigned int fail_code;
	struct Crypt_Skipjack_stats *i; // Not used
} Crypt_Skipjack;

struct Crypt_Skipjack_methods {
	void (*setkey)(Crypt_Skipjack *, unsigned char *);
	int (*encrypt)(Crypt_Skipjack *, unsigned char *, unsigned int size);
	int (*decrypt)(Crypt_Skipjack *, unsigned char *, unsigned int);
	void (*reset)(Crypt_Skipjack *);
};

void Crypt_Skipjack_ctor(Crypt_Skipjack *);
void Crypt_Skipjack_m_setkey(Crypt_Skipjack *, unsigned char *); // key is exactly 10 bytes or should be 
int Crypt_Skipjack_m_encrypt(Crypt_Skipjack *, unsigned char *, unsigned int); // Crypt a buffer
int Crypt_Skipjack_m_decrypt(Crypt_Skipjack *, unsigned char *, unsigned int); // Decrypt a buffer
void Crypt_Skipjack_m_reset(Crypt_Skipjack *); // Completely reset context for object re-use
void Crypt_Skipjack_dtor(Crypt_Skipjack *);
/* Crypt_Skipjack */
#endif
