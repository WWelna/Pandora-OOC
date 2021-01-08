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

/* This is a "test" executable messing around verifying various functionality */

#include "pandora.h"

char *test = "TEST TEST \0";
char test_buffer[256];

	/*Network *google = New(Network_ctor);
	char *request = "GET / HTTP/1.1\nHost: www.google.com\nConnection: close\n\n";
	char recv[4096];
	WSADATA wsainfo;
	WSAStartup(0x22, &wsainfo);
	myzero(recv, 4096);
	google->m->tcp_connect(google, "google.com\0", "80\0");
	if(google->fail_code==0) {
		fprintf(stderr, "[Connected]\n");
		google->m->send(google, request, mylen(request));
		fprintf(stderr, "[Request Sent]\n");
		do {
			if(google->m->recv(google, recv, 4095)>0) 
				fprintf(stdout, "[Received Data]\n\n%s\n\n", recv);
		} while(google->fail_code==0);
	} else {
		fprintf(stderr, "[Couldn't connect]\n");
	}
	fprintf(stdout, "total of %i bytes sent and %i bytes received\n", google->i->total_sent, google->i->total_recv);
	ResEdit *res = New(ResEdit_ctor);
	res->m->open(res, "testdll.dll");
	res->m->update(res, RT_RCDATA, MAKEINTRESOURCE(69), recv, mylen(recv));
	Delete(res);
	Delete(google);
	WSACleanup(); */
/*	Crypt_Skipjack *jack = New(Crypt_Skipjack_ctor);
	unsigned char key[10] = { 0x00, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 };
	unsigned char inp[8] = { 0x33, 0x22, 0x11, 0x00, 0xdd, 0xcc, 0xbb, 0xaa };
	char *string_test = "TESTTESTTESTTESTTESTTESTTEST";
	int x, len=mylen(string_test);
	myzero(test_buffer, 256);
	mycpy(test_buffer, string_test, len);
	jack->m->setkey(jack, key);
	dump_array_2("Decrypted", test_buffer, len+8);
	x=jack->m->encrypt(jack, test_buffer, len);
	dump_array_2("Encrypted", test_buffer, x);
	jack->m->decrypt(jack, test_buffer, x);
	dump_array_2("Decrypted", test_buffer, len+8);
	fprintf(stdout, "%i -> %s\n", len, test_buffer);
	Delete(jack); */
int main(int argc, char **argv) {
	if(!pandora_boot())
		return 1;
	Proxy *proxy = New(Proxy_ctor);
	char test[1024];
	uint32_t a=0;
	int x;
	myzero(test, 256);
	strcpy(test, "GET /test HTTP/1.1\nHost:google.com\n\n\0");
	proxy->m->socks5_connect(proxy, "127.0.0.1", "9050", "www.google.com", "80");
	proxy->m->send(proxy, test, mylen(test));
	x=proxy->m->recv(proxy, test, 1024);
	fprintf(stdout, "\n%s\n%i", test, x);
	Delete(proxy);
	return 0;
}
