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

#ifndef __OOCNETWORK__
#define __OOCNETWORK__
#include "pandora.h"

struct Network_data {
	SOCKET sock;
	char host[256]; // Unlikely to be longer
	char port[6]; // 1-65535
	struct timeval tv; // for send and recv operations
	uint8_t attempt_reconnect;
	uint8_t a_backlog, a_mode, a_socktype, a_protocol; // reconnect info
};

struct Network_add {
	/* Some additional info for reporting */
	uint64_t total_sent, total_recv; // Why not
	uint32_t last_sent, last_recv; // For possible usages
};

struct Network_methods;
typedef struct {
	void (*new)(void *);
	struct Network_data *data;
	struct Network_methods *m;
	void (*delete)(void *);
	void (*fail)(void *, unsigned int);
	unsigned int fail_code;
	struct Network_add *i;
} Network;

struct Network_methods {
	int (*reconnect)(Network *);
	void (*tcp_connect)(Network *, char *, char *);
	void (*udp_connect)(Network *, char *, char *);
	void (*connect)(Network *, char *, char *, uint32_t, uint32_t);
	void (*listen)(Network *, char *, char *, uint32_t, uint32_t, uint32_t);
	uint32_t (*send)(Network *, char *, uint32_t);
	uint32_t (*recv)(Network *, char *, uint32_t);
	int (*ipv4_resolve2addr)(Network *, char *, uint32_t *);
};

void Network_exception(Network *, unsigned int);
void Network_ctor(Network *);
int Network_m_reconnect(Network *);
void Network_m_tcp_connect(Network *, char *, char *);
void Network_m_udp_connect(Network *, char *, char *);
void Network_m_connect(Network *, char *, char *, uint32_t, uint32_t);
void Network_m_listen(Network *, char *, char *, uint32_t, uint32_t, uint32_t);
uint32_t Network_m_send(Network *, char *, uint32_t);
uint32_t Network_m_recv(Network *, char *, uint32_t);
int Network_m_ipv4_resolve2addr(Network *, char *, uint32_t *);
void Network_dtor(Network *);

#endif
