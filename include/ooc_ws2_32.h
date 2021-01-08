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

#ifndef __OOCWINSOCK__
#define __OOCWINSOCK__

struct Winsock_funcp {
	void *WSASocket_fps, *connect_fps, *select_fps, *send_fps, *recv_fps, *htons_fps, *gethostbyname_fps, *getaddrinfo_fps, *freeaddrinfo_fps, *closesocket_fps;
	void *WSAStartup_fps, *WSACleanup_fps; // Useless
};

struct Winsock_data {
	HMODULE dll;
	WSADATA wsainfo;
	struct Winsock_funcp fps;
};

struct Winsock_methods;
struct Winsock_calls;
typedef struct {
	void (*new)(void *);
	struct Winsock_data *data;
	struct Winsock_methods *m;
	void (*delete)(void *);
	void (*fail)(void *, unsigned int);
	unsigned int fail_code;
	struct Winsock_calls *c;
} Winsock;

struct Winsock_methods {
	int (*isloaded)(Winsock *);
	int (*load)(Winsock *);
	void (*unload)(Winsock *);
};

struct Winsock_calls {
	SOCKET (*WSASocket)(Winsock *, int, int, int, LPWSAPROTOCOL_INFO, GROUP, DWORD);
	int (*connect)(Winsock *, SOCKET, const struct sockaddr *, int);
	int (*select)(Winsock *, int, fd_set *, fd_set *, fd_set *, const struct timeval *);
	int (*send)(Winsock *, SOCKET, const char *, int, int);
	int (*recv)(Winsock *, SOCKET, char *, int, int);
	unsigned short (*htons)(Winsock *, unsigned short);
	struct hostent *(*gethostbyname)(Winsock *, const char *);
	int (*getaddrinfo)(Winsock *, char *, char *, const struct addrinfo *, struct addrinfo **);
	void (*freeaddrinfo)(Winsock *, struct addrinfo *);
	int (*closesocket)(Winsock *, SOCKET);
};

void Winsock_ctor(Winsock *);

int Winsock_m_isloaded(Winsock *);
int Winsock_m_load(Winsock *);
void Winsock_m_unload(Winsock *);

SOCKET Winsock_c_WSASocket(Winsock *, int, int, int, LPWSAPROTOCOL_INFO, GROUP, DWORD);
int Winsock_c_connect(Winsock *, SOCKET, const struct sockaddr *, int);
int Winsock_c_select(Winsock *, int, fd_set *, fd_set *, fd_set *, const struct timeval *);
int Winsock_c_send(Winsock *, SOCKET, const char *, int, int);
int Winsock_c_recv(Winsock *, SOCKET, char *, int, int);
unsigned short Winsock_c_htons(Winsock *, unsigned short);
struct hostent *Winsock_c_gethostbyname(Winsock *, const char *);
int Winsock_c_getaddrinfo(Winsock *, char *, char *, const struct addrinfo *, struct addrinfo **);
void Winsock_c_freeaddrinfo(Winsock *, struct addrinfo *);
int Winsock_c_closesocket(Winsock *, SOCKET);

void Winsock_dtor(Winsock *);

#endif
