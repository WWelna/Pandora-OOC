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

#ifndef __OOCPROXY__
#define __OOCPROXY__
#include "pandora.h"

#define PROXY_HTTP    0x01
#define PROXY_SOCKS4  0x04
#define PROXY_SOCKS4a 0x4a
#define PROXY_SOCKS5  0x05

#define TMP_SIZE 1024
#define LOGIN_SIZE 128

struct Proxy_data {
	Network *proxy_connect;
	char proxy_host[256];
	char proxy_port[6];
	char dest_host[256];
	char dest_port[6];
	char login_user[LOGIN_SIZE];
	char login_pass[LOGIN_SIZE];
	char tmp[TMP_SIZE]; // for random things
	uint8_t type;
	uint8_t is_connected;
};

struct Proxy_methods;
typedef struct {
	void (*new)(void *);
	struct Proxy_data *data;
	struct Proxy_methods *m;
	void (*delete)(void *);
	void (*fail)(void *, unsigned int);
	unsigned int fail_code;
	void *a;	
} Proxy;

struct Proxy_methods {
	int (*http_connect)(Proxy *, char *, char *, char *, char *);
	int (*socks4_connect)(Proxy *, char *, char *, char *, char *);
	int (*socks4a_connect)(Proxy *, char *, char *, char *, char *);
	int (*socks5_connect)(Proxy *, char *, char *, char *, char *);
	int (*reconnect)(Proxy *);
	void (*userpass)(Proxy *, char *, char *);
	int (*send)(Proxy *, char *, uint32_t);
	int (*recv)(Proxy *, char *, uint32_t);
};

void Proxy_ctor(Proxy *);
int Proxy_m_http_connect(Proxy *, char *, char *, char *, char *);
int Proxy_m_socks4_connect(Proxy *, char *, char *, char *, char *);
int Proxy_m_socks4a_connect(Proxy *, char *, char *, char *, char *);
int Proxy_m_socks5_connect(Proxy *, char *, char *, char *, char *);
int Proxy_m_reconnect(Proxy *);
void Proxy_m_userpass(Proxy *, char *, char *);
int Proxy_m_send(Proxy *, char *, uint32_t);
int Proxy_m_recv(Proxy *, char *, uint32_t);
void Proxy_dtor(Proxy *);

#endif
