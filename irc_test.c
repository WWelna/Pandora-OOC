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

#include "pandora.h"

#define irc_server "irc.server.net"
#define irc_port "6667"

String *random_string() {
	String *s = New(String_ctor);
	int x, rand_len = 4;
	s->m->alloc(s, 17); // 16 char string + 1 for zero term
	rand_len += rand()%12;
	for(x=0; x < rand_len; ++x)
		s->m->sindex(s, x, 0x61+(rand()%26));
	return s;
}

String *build_login() {
	String *rand = random_string();
	String *login = New(String_ctor);
	login->m->append(login, "USER ");
	login->m->append(login, rand->m->data(rand));
	login->m->append(login, " 8 * :");
	login->m->append(login, rand->m->data(rand));
	login->m->append(login, "\n");
	Delete(rand); 
	return login;
}

String *build_nick() {
	String *rand = random_string();
	String *nick = New(String_ctor);
	nick->m->append(nick, "NICK ");
	nick->m->append(nick, rand->m->data(rand));
	nick->m->append(nick, "\n");
	Delete(rand);
	return nick;
}

int irc_login(Proxy *proxy) {
	int r, f=0;
	String *login, *tmp = New(String_ctor);
	tmp->m->alloc(tmp, 1025); // make a 1025 byte long sexy zero'ed tmp string, also abuse the string object
	while((r=proxy->m->recv(proxy, tmp->m->data(tmp), 1024))) {
		puts(tmp->m->data(tmp));
		if(tmp->m->find(tmp, "hostname"))
			break;
		tmp->m->zero(tmp);
	}
	if(r==0) {
		Delete(tmp);
		return 0; // 30 second timeout reached
	}
	
	login = build_nick(); // build login string
	proxy->m->send(proxy, login->m->data(login), login->m->size(login)); // send nick
	Delete(login); // no longer needed
	
	login = build_login(); // build login string
	proxy->m->send(proxy, login->m->data(login), login->m->size(login)); // send user
	Delete(login); // no longer needed

	tmp->m->zero(tmp);
	while((r=proxy->m->recv(proxy, tmp->m->data(tmp), 1024))) {
		char *p;
		puts(tmp->m->data(tmp));
		if(tmp->m->find(tmp, "End of /MOTD command")) {
			f=1; // set successfully got MOTD banner
			break;
		}
		if((p=(char *)tmp->m->find(tmp, "PING :"))) {
			String *tmp2 = New(String_ctor);
			tmp2->m->setl(tmp2, p);
			tmp2->m->sindex(tmp2, 1, 'O');
			proxy->m->send(proxy, tmp2->m->data(tmp2), tmp2->m->size(tmp2)); // send PONG
			Delete(tmp2);
		}
		tmp->m->zero(tmp);
	}
	if(f==0) {
		Delete(tmp);
		return 0; // connection closed, ping timeout, or glined
	}
	Delete(tmp);
	return 1; // We Are In!
}

void ircmsg(Proxy *p, char *msg) {
	Sleep(300);
	p->m->send(p, msg, mylen(msg));
}

void lol_nick(Proxy *proxy) {
	String *nick = build_nick();
	ircmsg(proxy, nick->m->data(nick));
	Delete(nick);
	Sleep(400);
}

void irc_connect() {
	Proxy *proxy = New(Proxy_ctor); // New Proxy/Network Object YAYS!
	String *tmp = New(String_ctor);
	tmp->m->alloc(tmp, 4096);
	proxy->m->socks5_connect(proxy, "127.0.0.1", "9050", irc_server, irc_port); // Tor Connect
	if(proxy->fail_code == 0) { // no errors on object creation nor socks5 connection to irc
		if(!irc_login(proxy)) { // We are IN!
			ircmsg(proxy, "JOIN #2600\n");
			proxy->m->recv(proxy, tmp->m->data(tmp), 4096);
			puts(tmp->m->data(tmp));
			tmp->m->zero(tmp);
			ircmsg(proxy, "JOIN REDACTED\n");
			int x;
			for(x=0; x < 128; ++x) {
			lol_nick(proxy);
			/* ircmsg(proxy, "PRIVMSG REDACTED :=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
			ircmsg(proxy, "PRIVMSG REDACTED :              .,-:;//;:=,\n");
			ircmsg(proxy, "PRIVMSG REDACTED :          . :H@@@MM@M#H/.,+%;,\n");
			ircmsg(proxy, "PRIVMSG REDACTED :       ,/X+ +M@@M@MM%=,-%HMMM@X/,\n");
			ircmsg(proxy, "PRIVMSG REDACTED :     -+@MM; $M@@MH+-,;XMMMM@MMMM@+-\n");
			ircmsg(proxy, "PRIVMSG REDACTED :    ;@M@@M- XM@X;. -+XXXXXHHH@M@M#@/.\n");
			ircmsg(proxy, "PRIVMSG REDACTED :  ,%MM@@MH ,@%=            .---=-=:=,.\n");
			ircmsg(proxy, "PRIVMSG REDACTED :  =@#@@@MX .,              -%HX$$%%%+;\n");
			ircmsg(proxy, "PRIVMSG REDACTED : =-./@M@M$                  .;@MMMM@MM:\n");
			ircmsg(proxy, "PRIVMSG REDACTED : X@/ -$MM/                    .+MM@@@M$\n");
			ircmsg(proxy, "PRIVMSG REDACTED :,@M@H: :@:                    . =X#@@@@-\n");
			ircmsg(proxy, "PRIVMSG REDACTED :,@@@MMX, .                    /H- ;@M@M=\n");
			ircmsg(proxy, "PRIVMSG REDACTED :.H@@@@M@+,                    %MM+..%#$.\n");
			ircmsg(proxy, "PRIVMSG REDACTED : /MMMM@MMH/.                  XM@MH; =; \n");
			ircmsg(proxy, "PRIVMSG REDACTED :  /%+%$XHH@$=              , .H@@@@MX,\n");
			ircmsg(proxy, "PRIVMSG REDACTED :   .=--------.           -%H.,@@@@@MX,\n");
			ircmsg(proxy, "PRIVMSG REDACTED :   .%MM@@@HHHXX$$$%+- .:$MMX =M@@MM%.\n");
			ircmsg(proxy, "PRIVMSG REDACTED :     =XMMM@MM@MM#H;,-+HMM@M+ /MMMX=\n");
			ircmsg(proxy, "PRIVMSG REDACTED :       =%@M@M#@$-.=$@MM@@@M; %M%=\n");
			ircmsg(proxy, "PRIVMSG REDACTED :         ,:+$+-,/H#MMMMMMM@= =,\n");
			ircmsg(proxy, "PRIVMSG REDACTED :               =++%%%%+/:-.\n"); */
			ircmsg(proxy, "PING :LOL\n");
			proxy->m->recv(proxy, tmp->m->data(tmp), 4096);
			puts(tmp->m->data(tmp));
			tmp->m->zero(tmp);
			if(proxy->fail_code != 0)
				break;
			}
		}
	}
	Sleep(1000);
	ircmsg(proxy, "QUIT :X\n");
	Delete(proxy);
}

void *thread_spawn(void *n) {
	int x = (int)*n;
	srand(time(NULL)+69+x);
	for(x=0; x < 9999; ++x) {
		irc_connect();
		Sleep(1000);
	}
	return NULL;
}

int main(int argc, char **argv) {
	pandora_boot();
	HANDLE threads[1];
	int x=1, y=2, z=3;
	//threads[0] = CreateThread(NULL, 0, (void *)&thread_spawn, &x, 0, NULL);
	Sleep(5000);
	threads[0] = CreateThread(NULL, 0, (void *)&thread_spawn, &y, 0, NULL);
	//threads[2] = CreateThread(NULL, 0, (void *)&thread_spawn, &z, 0, NULL);
	WaitForMultipleObjects(1, threads, TRUE, INFINITE);
	return 0;
}
