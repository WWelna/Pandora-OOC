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

/* Encrypted Strings Information */
#include <ooc_ws2_32_crypt.c>
/* Encrypted Strings Information */

/* Function Typedefs */
typedef SOCKET (*WSASocketp)(int, int, int, LPWSAPROTOCOL_INFO, GROUP, DWORD);
typedef int (*connectp)(SOCKET, const struct sockaddr *, int);
typedef int (*selectp)(int, fd_set *, fd_set *, fd_set *, const struct timeval *);
typedef int (*sendp)(SOCKET, const char *, int, int);
typedef int (*recvp)(SOCKET, char *, int, int);
typedef unsigned short (*htonsp)(unsigned short);
typedef struct hostent *(*gethostbynamep)(const char *);
typedef int (*getaddrinfop)(char *, char *, const struct addrinfo *, struct addrinfo **);
typedef void (*freeaddrinfop)(struct addrinfo *);
typedef int (*closesocketp)(SOCKET);
typedef int (*WSAStartupp)(WORD, WSADATA *);
typedef int (*WSACleanupp)(void);
/* Function Typedefs */

static const struct {
	int (*isloaded)(Winsock *);
	int (*load)(Winsock *);
	void (*unload)(Winsock *);
} Winsock_methods_static = { &Winsock_m_isloaded, &Winsock_m_load, &Winsock_m_unload };

static const struct {
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
} Winsock_calls_static = { &Winsock_c_WSASocket, &Winsock_c_connect, &Winsock_c_select, &Winsock_c_send, &Winsock_c_recv, &Winsock_c_htons, &Winsock_c_gethostbyname, &Winsock_c_getaddrinfo, &Winsock_c_freeaddrinfo, &Winsock_c_closesocket };

void *Winsock_p_procload(Winsock *, char *, int, int, char *);

void Winsock_ctor(Winsock *ws) {
	ws->new = (void *)&Winsock_ctor;
	ws->delete = (void *)&Winsock_dtor;
	ws->m = myalloc(1, sizeof(struct Winsock_methods));
	if(!ws->m)
		ws->fail(ws, 1);
	else {
		mycpy(ws->m, (void *)&Winsock_methods_static, sizeof(struct Winsock_methods));
		ws->data = myalloc(1, sizeof(struct Winsock_data));
		if(!ws->data)
			ws->fail(ws, 2);
		else {
			ws->c = myalloc(1, sizeof(struct Winsock_calls));
			if(!ws->c)
				ws->fail(ws, 3);
			mycpy(ws->c, (void *)&Winsock_calls_static, sizeof(struct Winsock_calls));
		}
	}
}

int Winsock_m_isloaded(Winsock *ws) {
	if(ws->data->dll)
		return 1;
	else
		return 0;
}

int Winsock_m_load(Winsock *ws) {
	if(!ws->data->dll) {
		char *dllname = ooc_dec(WinsockDll_s, WINSOCKDLL_S_CRYPTSIZE, WINSOCKDLL_S_CALLSIZE, Winsock_cryptkey);
		if(dllname) {
			ws->data->dll = LoadLibrary(dllname);
			ooc_free(dllname, WINSOCKDLL_S_CRYPTSIZE); // Destroy right after usage & Zero Memory
			if(ws->data->dll) { // Success!
				WSAStartupp startup = (WSAStartupp)Winsock_p_procload(ws, WSAStartup_s, WSASTARTUP_S_CRYPTSIZE, WSASTARTUP_S_CALLSIZE, Winsock_cryptkey);
				WSACleanupp cleanup = (WSACleanupp)Winsock_p_procload(ws, WSACleanup_s, WSACLEANUP_S_CRYPTSIZE, WSACLEANUP_S_CALLSIZE, Winsock_cryptkey);
				if(startup == NULL || cleanup == NULL) {
					ws->fail(ws, 802);
					return 0;
				}
				myzero(&ws->data->fps, sizeof(struct Winsock_funcp)); // Make sure pointer table is empty
				ws->fail_code = 0; // If exception was ever set clear it
				ws->data->fps.WSAStartup_fps = startup;
				ws->data->fps.WSACleanup_fps = cleanup;
				startup(0x22, &ws->data->wsainfo); // do startup
				return 1;
			} else { // Couldn't load dll
				ws->fail(ws, 800);
				return 0;
			}
		} else { // Memory No Allocate
			ws->fail(ws, 801);
		}
	} else
		return 1; // already loaded
}

void Winsock_m_unload(Winsock *ws) {
	ws->delete(ws);
}

void *Winsock_p_procload(Winsock *ws, char *crypt_string, int crypt_size, int crypt_call, char *crypt_key) {
	char *procname = ooc_dec(crypt_string, crypt_size, crypt_call, crypt_key);
	if(procname) {
		void *p = GetProcAddress(ws->data->dll, procname);
		ooc_free(procname, crypt_size); // Take care of string
		return p; // callee will check if this is null or not
	} else
		return NULL;
}

SOCKET Winsock_c_WSASocket(Winsock *ws, int p1, int p2, int p3, LPWSAPROTOCOL_INFO p4, GROUP p5, DWORD p6) {
	if(ws->fail_code == 0 && ws->m->isloaded(ws)) {
		WSASocketp X;
		if(ws->data->fps.WSASocket_fps == NULL) {
			X = (WSASocketp)Winsock_p_procload(ws, WSASocket_s, WSASOCKET_S_CRYPTSIZE, WSASOCKET_S_CALLSIZE, Winsock_cryptkey);
			ws->data->fps.WSASocket_fps = X;
		} else
			X = (WSASocketp)ws->data->fps.WSASocket_fps;
		if(X)
			return X(p1, p2, p3, p4, p5, p6);
		else
			ws->fail(ws, 810);
	}
	return INVALID_SOCKET;
}

int Winsock_c_connect(Winsock *ws, SOCKET p1, const struct sockaddr *p2, int p3) {
	if(ws->fail_code == 0 && ws->m->isloaded(ws)) {
		connectp X;
		if(ws->data->fps.connect_fps == NULL) {
			X = (connectp)Winsock_p_procload(ws, connect_s, CONNECT_S_CRYPTSIZE, CONNECT_S_CALLSIZE, Winsock_cryptkey);
			ws->data->fps.connect_fps = X;
		} else
			X = (connectp)ws->data->fps.connect_fps;
		if(X)
			return X(p1, p2, p3);
		else
			ws->fail(ws, 811);
	}
	return SOCKET_ERROR;
}

int Winsock_c_select(Winsock *ws, int p1, fd_set *p2, fd_set *p3, fd_set *p4, const struct timeval *p5) {
	if(ws->fail_code == 0 && ws->m->isloaded(ws)) {
		selectp X;
		if(ws->data->fps.select_fps == NULL) {
			X = (selectp)Winsock_p_procload(ws, select_s, SELECT_S_CRYPTSIZE, SELECT_S_CALLSIZE, Winsock_cryptkey);
			ws->data->fps.select_fps = X;
		} else
			X = (selectp)ws->data->fps.select_fps;
		if(X)
			return X(p1, p2, p3, p4, p5);
		else
			ws->fail(ws, 812);
	}
	return 0;
}

int Winsock_c_send(Winsock *ws, SOCKET p1, const char *p2, int p3, int p4) {
	if(ws->fail_code == 0 && ws->m->isloaded(ws)) {
		sendp X;
		if(ws->data->fps.send_fps == NULL) {
			X = (sendp)Winsock_p_procload(ws, send_s, SEND_S_CRYPTSIZE, SEND_S_CALLSIZE, Winsock_cryptkey);
			ws->data->fps.send_fps = X;
		} else
			X = (sendp)ws->data->fps.send_fps;
		if(X)
			return X(p1, p2, p3, p4);
		else
			ws->fail(ws, 813);
	}
	return 0;
}

int Winsock_c_recv(Winsock *ws, SOCKET p1, char *p2, int p3, int p4) {
	if(ws->fail_code == 0 && ws->m->isloaded(ws)) {
		recvp X;
		if(ws->data->fps.recv_fps == NULL) {
			X = (recvp)Winsock_p_procload(ws, recv_s, RECV_S_CRYPTSIZE, RECV_S_CALLSIZE, Winsock_cryptkey);
			ws->data->fps.recv_fps = X;
		} else
			X = (recvp)ws->data->fps.recv_fps;
		if(X)
			return X(p1, p2, p3, p4);
		else
			ws->fail(ws, 814);
	}
	return 0;
}

unsigned short Winsock_c_htons(Winsock *ws, unsigned short p1) {
	if(ws->fail_code == 0 && ws->m->isloaded(ws)) {
		htonsp X;
		if(ws->data->fps.htons_fps == NULL) {
			X = (htonsp)Winsock_p_procload(ws, htons_s, HTONS_S_CRYPTSIZE, HTONS_S_CALLSIZE, Winsock_cryptkey);
			ws->data->fps.htons_fps = X;
		} else
			X = (htonsp)ws->data->fps.htons_fps;
		if(X)
			return X(p1);
		else
			ws->fail(ws, 815);
	}
	return 0;
}

struct hostent *Winsock_c_gethostbyname(Winsock *ws, const char *p1) {
	if(ws->fail_code == 0 && ws->m->isloaded(ws)) {
		gethostbynamep X;
		if(ws->data->fps.gethostbyname_fps == NULL) {
			X = (gethostbynamep)Winsock_p_procload(ws, gethostbyname_s, GETHOSTBYNAME_S_CRYPTSIZE, GETHOSTBYNAME_S_CALLSIZE, Winsock_cryptkey);
			ws->data->fps.gethostbyname_fps = X;
		} else
			X = (gethostbynamep)ws->data->fps.gethostbyname_fps;
		if(X)
			return X(p1);
		else
			ws->fail(ws, 816);
	}
	return NULL;
}

int Winsock_c_getaddrinfo(Winsock *ws, char *p1, char *p2, const struct addrinfo *p3, struct addrinfo **p4) {
	if(ws->fail_code == 0 && ws->m->isloaded(ws)) {
		getaddrinfop X;
		if(ws->data->fps.getaddrinfo_fps == NULL) {
			X = (getaddrinfop)Winsock_p_procload(ws, getaddrinfo_s, GETADDRINFO_S_CRYPTSIZE, GETADDRINFO_S_CALLSIZE, Winsock_cryptkey);
			ws->data->fps.getaddrinfo_fps = X;
		} else
			X = (getaddrinfop)ws->data->fps.getaddrinfo_fps;
		if(X)
			return X(p1, p2, p3, p4);
		else
			ws->fail(ws, 817);
	}
	return -1;
}

void Winsock_c_freeaddrinfo(Winsock *ws, struct addrinfo *p1) {
	if(ws->fail_code == 0 && ws->m->isloaded(ws)) {
		freeaddrinfop X;
		if(ws->data->fps.freeaddrinfo_fps == NULL) {
			X = (freeaddrinfop)Winsock_p_procload(ws, freeaddrinfo_s, FREEADDRINFO_S_CRYPTSIZE, FREEADDRINFO_S_CALLSIZE, Winsock_cryptkey);
			ws->data->fps.freeaddrinfo_fps = X;
		} else
			X = (freeaddrinfop)ws->data->fps.freeaddrinfo_fps;
		if(X)
			X(p1);
		else
			ws->fail(ws, 818);
	}
}

int Winsock_c_closesocket(Winsock *ws, SOCKET p1) {
	if(ws->fail_code == 0 && ws->m->isloaded(ws)) {
		closesocketp X;
		if(ws->data->fps.closesocket_fps == NULL) {
			X = (closesocketp)Winsock_p_procload(ws, closesocket_s, CLOSESOCKET_S_CRYPTSIZE, CLOSESOCKET_S_CALLSIZE, Winsock_cryptkey);
			ws->data->fps.closesocket_fps = X;
		} else
			X = (closesocketp)ws->data->fps.closesocket_fps;
		if(X)
			return X(p1);
		else
			ws->fail(ws, 819);
	}
}

void Winsock_dtor(Winsock *ws) {
	if(ws->data->dll) {
		WSACleanupp cleanup = ws->data->fps.WSACleanup_fps;
		cleanup();
		FreeLibrary(ws->data->dll);
		ws->data->dll = NULL;
	}
	myzero(&ws->data->fps, sizeof(struct Winsock_funcp));
}
