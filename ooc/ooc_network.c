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

static const struct {
	int (*reconnect)(Network *);
	void (*tcp_connect)(Network *, char *, char *);
	void (*udp_connect)(Network *, char *, char *);
	void (*connect)(Network *, char *, char *, uint32_t, uint32_t);
	void (*listen)(Network *, char *, char *, uint32_t, uint32_t, uint32_t);
	uint32_t (*send)(Network *, char *, uint32_t);
	uint32_t (*recv)(Network *, char *, uint32_t);
	int (*ipv4_resolve2addr)(Network *, char *, uint32_t *);
} Network_methods_static = { &Network_m_reconnect, &Network_m_tcp_connect, &Network_m_udp_connect, &Network_m_connect, &Network_m_listen, &Network_m_send, &Network_m_recv, &Network_m_ipv4_resolve2addr };

void Network_exception(Network *n, unsigned int code) {
	if(n) {
		n->fail_code = code;	
		// Attempt reconnect and is recoverable do not call dtor
		if(n->data && n->data->attempt_reconnect==1 && (code==100 || code==101 || code==200 || code==201 || code==301 || code==302)) {
			// Do Something?
		} //else if(code == 202) { // Connection Closed so do nothing
		//} else // If not recoverable error or attempt_reconnect not set call destructor
			//if(n && n->delete)
				//n->delete(n); 
	}
}

void Network_ctor(Network *n) {
	n->new = (void *)&Network_ctor;
	n->delete = (void *)&Network_dtor;
	n->fail = (void *)&Network_exception; // Custom Exception Handler Weeee!
	n->m = myalloc(1, sizeof(struct Network_methods));
	if(!n->m)
		n->fail(n, 1);
	else {
		mycpy((void *)n->m, (void *)&Network_methods_static, sizeof(struct Network_methods));
		n->data = myalloc(1, sizeof(struct Network_data));
		if(!n->data)
			n->fail(n, 2);
		else {
			n->i = myalloc(1, sizeof(struct Network_add));
			if(!n->i)
				n->fail(n, 3);
			else {
				n->data->tv.tv_sec = 7; n->data->tv.tv_usec = 0; // set default to 7 seconds
				n->data->a_backlog = 15; // set default backlog to 15
			}
		}
	}
}

int Network_m_reconnect(Network *n) { // Attempt to recover a connection and reset exception if success
	if(n->data->sock != INVALID_SOCKET && n->data->sock>0)
		WSDLL->c->closesocket(WSDLL, n->data->sock);
	gen_fail old_handler = (gen_fail)n->fail;
	n->fail = (void *)&gen_exception_noop;
	n->fail_code = 0;
	if(n->data->a_mode)
		n->m->connect(n, n->data->host, n->data->port, n->data->a_socktype, n->data->a_protocol); // Attempt the great reconnect
	else
		n->m->listen(n, n->data->host, n->data->port, n->data->a_backlog, n->data->a_socktype, n->data->a_protocol); // Attempt the great listen
	n->fail = old_handler; // Restore original exception handler
	if(n->fail_code == 0) { // Reconnect Successful
		return 1;
	} else { // Fail
		n->fail(n, 302); //  Throw 302 reconnect failed
		return 0;
	}
}

void Network_m_tcp_connect(Network *n, char *host, char *port) {
	n->data->a_socktype = SOCK_STREAM;
	n->data->a_protocol = IPPROTO_TCP;
	n->data->a_mode = 1; // 1 = connect
	n->m->connect(n, host, port, SOCK_STREAM, IPPROTO_TCP); // If there is an error or exception it will drag down 
}

void Network_m_udp_connect(Network *n, char *host, char *port) {
	n->data->a_socktype = SOCK_DGRAM;
	n->data->a_protocol = IPPROTO_UDP;
	n->data->a_mode = 1; // 1 = connect
	n->m->connect(n, host, port, SOCK_DGRAM, IPPROTO_UDP);
}

void Network_m_connect(Network *n, char *host, char *port, uint32_t family, uint32_t proto) {
	if(n->fail_code==0) {
		struct addrinfo hints, *servinfo, *p=NULL;
		int r;
		myzero(&hints, sizeof(hints));
		if(mylen(n->data->host)==0 && mylen(n->data->port)==0) {
			mycpy(n->data->host, host, mylen(host));
			mycpy(n->data->port, port, mylen(port));
		}
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = family;
		hints.ai_protocol = proto;
		if((r=WSDLL->c->getaddrinfo(WSDLL, host, port, &hints, &servinfo))!=0) {
			n->fail(n, 300);
		} else {
			for(p=servinfo; p!=NULL; p=p->ai_next) {
				if((n->data->sock=WSDLL->c->WSASocket(WSDLL, p->ai_family, p->ai_socktype, p->ai_protocol, NULL, 0, 0)) == INVALID_SOCKET)
					continue;
				if(WSDLL->c->connect(WSDLL, n->data->sock, p->ai_addr, p->ai_addrlen) == SOCKET_ERROR) {
					WSDLL->c->closesocket(WSDLL, n->data->sock); n->data->sock = 0;
					continue;
				}
				break;
			}
			if(p == NULL) {
				if(servinfo)
					WSDLL->c->freeaddrinfo(WSDLL, servinfo);
				n->fail(n, 301);
			} else {
				if(servinfo)
					WSDLL->c->freeaddrinfo(WSDLL, servinfo);
			}
		}
	}
}

void Network_m_listen(Network *n, char *host, char *port, uint32_t backlog, uint32_t a, uint32_t b) {
}

uint32_t Network_m_send(Network *n, char *b, uint32_t s) {
	if(n->fail_code==0) {
		fd_set fds;
		int r;
		FD_ZERO(&fds);
		FD_SET(n->data->sock, &fds);
		if((r=WSDLL->c->select(WSDLL, 0, NULL, &fds, NULL, &n->data->tv))!=SOCKET_ERROR) {
			if(FD_ISSET(n->data->sock, &fds)) {
				r=WSDLL->c->send(WSDLL, n->data->sock, b, s, 0);
				n->i->total_sent += r;
				n->i->last_sent = r;
				return r;
			} else if(r==0)
				return 0; // Timeout
			else {
				n->fail(n, 101); // wtf?
				return 0;
			}
		} else {
			n->fail(n, 100);
			return 0;
		}
	} else
		return 0;
}

uint32_t Network_m_recv(Network *n, char *b, uint32_t s) {
	if(n->fail_code==0) {
		fd_set fds;
		int r;
		FD_ZERO(&fds);
		FD_SET(n->data->sock, &fds);
		if((r=WSDLL->c->select(WSDLL, 0, &fds, NULL, NULL, &n->data->tv))!=SOCKET_ERROR) {
			if(FD_ISSET(n->data->sock, &fds)) {
				r=WSDLL->c->recv(WSDLL, n->data->sock, b, s, MSG_PEEK); // peek!
				if(r<=0) { // socket was closed
					n->fail(n, 202); // Raise exception indicating socket was closed
					return 0; // just return 0
				} else {
					r=WSDLL->c->recv(WSDLL, n->data->sock, b, s, 0);
					n->i->total_recv = n->i->total_recv + r;
					n->i->last_recv = r;
					return r; // just return total bytes recv'ed
				}
			} else if(r==0)
				return 0; // Timeout
			else {
				n->fail(n, 201); // wtf?
				return 0;
			}
		} else {
			n->fail(n, 200);
			return 0;
		}
	} else
		return 0;
}

int Network_m_ipv4_resolve2addr(Network *n, char *host, uint32_t *addr) {
	if(host != NULL && addr != NULL) {
		PHOSTENT ent = WSDLL->c->gethostbyname(WSDLL, host);
		if(ent) { // return first entry
			if(ent->h_addrtype == AF_INET) {
				mycpy(addr, ent->h_addr, 4);
				return 1;
			} else
				return 0;
		} else 
			return 0;
	}
	return 0;
}

void Network_dtor(Network *o) {
	if(o->data) 
		if(o->data->sock != INVALID_SOCKET && o->data->sock>0)
			WSDLL->c->closesocket(WSDLL, o->data->sock);
}
