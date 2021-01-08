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
	int (*http_connect)(Proxy *, char *, char *, char *, char *);
	int (*socks4_connect)(Proxy *, char *, char *, char *, char *);
	int (*socks4a_connect)(Proxy *, char *, char *, char *, char *);
	int (*socks5_connect)(Proxy *, char *, char *, char *, char *);
	int (*Proxy_reconnect)(Proxy *);
	void (*userpass)(Proxy *, char *, char *);
	int (*send)(Proxy *, char *, uint32_t);
	int (*recv)(Proxy *, char *, uint32_t);
} Proxy_methods_static = { &Proxy_m_http_connect, &Proxy_m_socks4_connect, &Proxy_m_socks4a_connect, &Proxy_m_socks5_connect, &Proxy_m_reconnect, &Proxy_m_userpass, &Proxy_m_send, &Proxy_m_recv };

void Proxy_exception(Proxy *p, unsigned int code) {
	p->data->is_connected = 0; // set is_connected to 0 to prevent further recv and sends
	gen_exception((generic_object *)p, code); // pass to generic exception handler per usual
}

void Proxy_ctor(Proxy *p) {
	p->new = (void *)&Proxy_ctor;
	p->delete = (void *)&Proxy_dtor;
	p->fail = (void *)&Proxy_exception;
	p->m = myalloc(1, sizeof(struct Proxy_methods));
	if(!p->m)
		p->fail(p, 1);
	else {
		mycpy(p->m, (void *)&Proxy_methods_static, sizeof(struct Proxy_methods));
		p->data = myalloc(1, sizeof(struct Proxy_data));
		if(!p->data)
			p->fail(p, 2);
	}
}

int Proxy_m_http_connect(Proxy *p, char *phost, char *pport, char *dhost, char *dport) {
	return 0;
}

int Proxy_m_socks4_connect(Proxy *p, char *phost, char *pport, char *dhost, char *dport) {
	Network *n;
	if(p->data->proxy_connect)
		Delete(p->data->proxy_connect);
	n = New(Network_ctor);
	p->data->proxy_connect = n;
	n->m->tcp_connect(n, phost, pport);
	if(n->fail_code == 0) {
		char *tmp = p->data->tmp;
		uint32_t addr; uint16_t port = WSDLL->c->htons(WSDLL, myatoi(dport));
		if(Network_m_ipv4_resolve2addr(NULL, dhost, &addr)) {
			tmp[0] = 0x04; tmp[1] = 0x01; // socks4 connect
			mycpy(tmp+2, &port, 2);
			mycpy(tmp+4, &addr, 4);
			//tmp[8] = 'X'; tmp[9] = 0; // Username X
			if(n->m->send(n, tmp, 10)==0) { // send teh requests! 
					if(n->fail_code>0) {
						p->fail(p, n->fail_code);
						return 0;
					} else { // timed out
						p->fail(p, 110); // Timeout
						return 0;
					}
			}
			n->data->tv.tv_sec = 120; // Set connect timeout to 2 minutes
			if(n->m->recv(n, tmp, TMP_SIZE)==0) { 
				if(n->fail_code>0) {
					p->fail(p, n->fail_code);
					return 0;
				} else { // timed out
					p->fail(p, 210);
					return 0;
				}
			}
			if(tmp[1] != 0x5a) {
				p->fail(p, 601); // request failed
				return 0;
			}
		} else { // unable to resolve dest host
			p->fail(p, 600);
			return 0;
		}
	} else {
		p->fail(p, n->fail_code); // Some kind of connection fail
		return 0;
	}
	n->data->tv.tv_sec = 30;
	p->data->type = PROXY_SOCKS4;
	p->data->is_connected = 1;
	return 1;
}

int Proxy_m_socks4a_connect(Proxy *p, char *phost, char *pport, char *dhost, char *dport) {
	Network *n;
	if(p->data->proxy_connect)
		Delete(p->data->proxy_connect);
	n = New(Network_ctor);
	p->data->proxy_connect = n;
	n->m->tcp_connect(n, phost, pport);
	if(n->fail_code == 0) {
		char *tmp = p->data->tmp;
		uint16_t port = WSDLL->c->htons(WSDLL, myatoi(dport));
		int l = mylen(dhost);
		tmp[0] = 0x04; tmp[1] = 0x01; // socks4 connect
		mycpy(tmp+3, &port, 2);
		tmp[4] = 0x00; tmp[5] = 0x00; tmp[6] = 0x00; tmp[7] = 0x01; // Fake ip
		tmp[8] = 'X'; tmp[9] = 0; // Username X
		mycpy(tmp+11, dhost, l); l += 12; tmp[l] = 0; // Append hostname
		if(n->m->send(n, tmp, l)==0) { // send teh requests! 
				if(n->fail_code>0) {
					p->fail(p, n->fail_code);
					return 0;
				} else { // timed out
					p->fail(p, 110); // Timeout
					return 0;
				}
		}
		n->data->tv.tv_sec = 120; // Set connect timeout to 2 minutes
		if(n->m->recv(n, tmp, TMP_SIZE)==0) { 
			if(n->fail_code>0) {
				p->fail(p, n->fail_code);
				return 0;
			} else { // timed out
				p->fail(p, 210);
				return 0;
			}
		}
		if(tmp[1] != 0x5a) {
			p->fail(p, 601); // request failed
			return 0;
		}
	} else {
		p->fail(p, n->fail_code); // Some kind of connection fail
		return 0;
	}
	n->data->tv.tv_sec = 30;
	p->data->type = PROXY_SOCKS4a;
	p->data->is_connected = 1;
	return 1;
}

int Proxy_m_socks5_connect(Proxy *p, char *phost, char *pport, char *dhost, char *dport) {
	Network *n;
	if(p->data->proxy_connect)
		Delete(p->data->proxy_connect);
	n = New(Network_ctor);
	p->data->proxy_connect = n;
	n->m->tcp_connect(n, phost, pport);
	if(n->fail_code == 0) {
		uint8_t s, l = mylen(dhost);
		uint16_t t;
		char *tmp = p->data->tmp;
		if(mylen(p->data->login_user)>0) {
			if(n->m->send(n, "\x05\x02\x00\x02", 4)==0) { // SOCKS 5 user/pwd auth 
				if(n->fail_code>0) {
					p->fail(p, n->fail_code);
					return 0;
				} else { // timed out
					p->fail(p, 110); // Timeout
					return 0;
				}
			}
		} else {
		if(n->m->send(n, "\x05\x01\x00", 3)==0) { // SOCKS 5 no auth 
				if(n->fail_code>0) {
					p->fail(p, n->fail_code);
					return 0;
				} else { // timed out
					p->fail(p, 110); // Timeout
					return 0;
				}
			}
		}
		if(n->m->recv(n, tmp, TMP_SIZE)==0) { // recieved nada
			if(n->fail_code>0) {
				p->fail(p, n->fail_code);
				return 0;
			} else { // timed out
				p->fail(p, 210);
				return 0;
			}
		}
		if((tmp[0] != 0x05) || (tmp[1] == 0xFF)) {
			p->fail(p, 601); // invalid response or no acceptable methods found
			return 0;
		}
		if(tmp[1] == 0x02) { // user/pwd auth
			uint8_t t1 = mylen(p->data->login_user);
			s=3; tmp[1] = t1;
			mycpy(tmp+s, p->data->login_user, t1); s+=t1+1;
			t1=mylen(p->data->login_pass);
			tmp[s] = t1; ++s;
			mycpy(tmp+s, p->data->login_pass, t1); s+=t1;
			if(n->m->send(n, tmp, s)==0) { // send user/pwd auth
				if(n->fail_code>0) {
					p->fail(p, n->fail_code);
					return 0;
				} else { // timed out
					p->fail(p, 111); // Timeout for proxy trying to connect
					return 0;
				}
			}
			if(n->m->recv(n, tmp, TMP_SIZE)==0) { // Ams I Auths?
				if(n->fail_code>0) {
					p->fail(p, n->fail_code);
					return 0;
				} else { // timed out
					p->fail(p, 211); // Timeout waiting to connect
					return 0;
				}
			}
			if(tmp[0] != 0x05 || tmp[1] != 0x00) {
				p->fail(p, 660); // bad user/pwd combo
				return 0;
			}
		} else if(tmp[1] != 0x00) { // unrecognized method
			p->fail(p, 602);
			return 0;
		}
		tmp[1] = 0x01; tmp[2] = 0x00; tmp[3] = 0x03; tmp[4] = l; // tmp[0] already equal to 0x05
		mycpy((tmp+5), dhost, l);
		t=WSDLL->c->htons(WSDLL, myatoi(dport));
		s=l+5;
		mycpy((tmp+s), &t, 2); s += 2;
		if(n->m->send(n, tmp, s)==0) { // send connect request
			if(n->fail_code>0) {
				p->fail(p, n->fail_code);
				return 0;
			} else { // timed out
				p->fail(p, 111); // Timeout for proxy trying to connect
				return 0;
			}
		}
		n->data->tv.tv_sec = 120; // Set connect timeout to 2 minutes
		if(n->m->recv(n, tmp, TMP_SIZE)==0) { // get response 
			if(n->fail_code>0) {
				p->fail(p, n->fail_code);
				return 0;
			} else { // timed out
				p->fail(p, 211); // Timeout waiting to connect
				return 0;
			}
		}
		if(tmp[0] != 0x05 && tmp[1] != 0x00) { // Connection Denied
			p->fail(p, 600);
			return 0;
		}
	} else {
		p->fail(p, n->fail_code); // Some kind of connection fail
		return 0;
	}
	n->data->tv.tv_sec = 30;
	p->data->type = PROXY_SOCKS5;
	p->data->is_connected = 1;
	return 1;
}

int Proxy_m_send(Proxy *p, char *b, uint32_t s) {
	if(p->data->is_connected && p->fail_code==0 && p->data->proxy_connect) {
		uint32_t ret = p->data->proxy_connect->m->send(p->data->proxy_connect, b, s);
		if(ret == 0) {
			if(p->data->proxy_connect->fail_code>0)
				p->fail(p, p->data->proxy_connect->fail_code);
			else
				return 0;
		} else
			return ret;
		return ret;
	} else {
		if(p->fail_code==0)
			p->fail(p, 112);
		return 0;
	}
	return 666;
}

int Proxy_m_recv(Proxy *p, char *b, uint32_t s) {
	if(p->data->is_connected && p->fail_code==0 && p->data->proxy_connect) {
		uint32_t ret = p->data->proxy_connect->m->recv(p->data->proxy_connect, b, s);
		if(ret == 0) {
			if(p->data->proxy_connect->fail_code>0)
				p->fail(p, p->data->proxy_connect->fail_code);
			else
				return 0;
			return ret;
		} else
			return ret;
	} else {
		if(p->fail_code==0)
			p->fail(p, 212);
		return 0;
	}
	return 666;
}

void Proxy_m_userpass(Proxy *p, char *user, char *pass) {
	if(user == NULL || pass == NULL) {
		myzero(p->data->login_user, LOGIN_SIZE);
		myzero(p->data->login_pass, LOGIN_SIZE);
	} else {
		int userlen=mylen(user), passlen=mylen(pass), maxlen=LOGIN_SIZE-1;
		if(userlen<LOGIN_SIZE)
			mycpy(p->data->login_user, user, userlen);
		else
			mycpy(p->data->login_user, user, maxlen);
		if(passlen<LOGIN_SIZE)
			mycpy(p->data->login_pass, pass, passlen);
		else
			mycpy(p->data->login_pass, pass, maxlen);
	}
}

int Proxy_m_reconnect(Proxy *p) {
}


void Proxy_dtor(Proxy *p) {
	if(p->data->proxy_connect) // if connection object exists delete it
		Delete(p->data->proxy_connect);
}
