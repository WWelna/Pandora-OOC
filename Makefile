# Pandora Makefile

# Config
FLAGS = "-Ic:\tcc\pandora\include" "-Ic:\tcc\pandora"
LIBS = -lkernel32 -lws2_32

# Objects
PRIMATIVES = asm_stuff.o mem.o string.o xxtea.o util.o fnv1a.o ooc.o crypt_call.o adler32.o crc32.o fastlz.o
OBJECTS = ooc_network.o ooc_proxy.o ooc_string.o ooc_data.o ooc_hashtable.o ooc_ws2_32.o pandora_boot.o
VARIOUS = pandora.o irc_test.o coffeexor.o pandora.exe irctest.exe xxtea.exe xxtea_asm.bin coffeexor-cli.exe
STRINGS = coffee.c ooc_ws2_32_crypt.c
SOURCE_FILES = primatives\xxtea_asm.asm primatives\asm_stuff.asm primatives\crypt_call.asm primatives\coffeexor.asm include\asm_stuff.h primatives\adler32.c primatives\crc32.c primatives\mem.c primatives\string.c primatives\xxtea.c primatives\util.c primatives\fnv1a.c ooc\ooc.c include\ooc.h ooc\ooc_network.c include\ooc_network.h ooc\ooc_proxy.c include\ooc_proxy.h ooc\ooc_string.c include\ooc_string.h ooc\ooc_skipjack.c include\ooc_skipjack.h ooc\ooc_resedit.c include\ooc_resedit.h ooc\ooc_data.c include\ooc_data.h ooc\ooc_hashtable.c include\ooc_hashtable.h ooc\ooc_ws2_32.c include\ooc_ws2_32.h pandora_boot.c tools\xxtea-cli.c tools\coffeexor-cli.c 

# Default Build just objects
framework : xxtea.exe $(PRIMATIVES) $(OBJECTS)

# Basics
asm_stuff.o : primatives\asm_stuff.asm
	nasm -felf -o asm_stuff.o primatives\asm_stuff.asm 
mem.o : primatives\mem.c
	tcc -c -o mem.o primatives\mem.c $(FLAGS)
string.o : primatives\string.c
	tcc -c -o string.o primatives\string.c $(FLAGS)
xxtea.o : coffee.c primatives\xxtea.c
	tcc -c -o xxtea.o primatives\xxtea.c $(FLAGS)
util.o : primatives\util.c
	tcc -c -o util.o primatives\util.c $(FLAGS)
fnv1a.o : primatives\fnv1a.c
	tcc -c -o fnv1a.o primatives\fnv1a.c $(FLAGS)
xxtea_asm.bin : primatives\xxtea_asm.asm
	nasm -fbin -o xxtea_asm.bin primatives\xxtea_asm.asm
coffee.c : xxtea_asm.bin coffeexor-cli.exe
	coffeexor-cli.exe xxtea_asm.bin > coffee.c
coffeexor.o : primatives\coffeexor.asm
	nasm -felf -o coffeexor.o primatives\coffeexor.asm
crypt_call.o : primatives\crypt_call.asm
	nasm -felf -o crypt_call.o primatives\crypt_call.asm
adler32.o : primatives\adler32.c
	tcc -c -o adler32.o primatives\adler32.c $(FLAGS)
crc32.o : primatives\crc32.c
	tcc -c -o crc32.o primatives\crc32.c $(FLAGS)
fastlz.o : primatives\fastlz.c
	tcc -c -o fastlz.o primatives\fastlz.c $(FLAGS)
# Strings
ooc_ws2_32_crypt.c : xxtea.exe
	openssl rand -out keys\ws2_pandora.bin 16
	xdump Winsock_cryptkey keys\ws2_pandora.bin > ooc_ws2_32_crypt.c
	xxtea /ekey:keys\ws2_pandora.bin /size:8 /ctag:WinsockDll_s "ws2_32.dll" >> ooc_ws2_32_crypt.c
	xxtea /ekey:keys\ws2_pandora.bin /size:8 /ctag:WSASocket_s "WSASocketA" >> ooc_ws2_32_crypt.c
	xxtea /ekey:keys\ws2_pandora.bin /size:8 /ctag:connect_s "connect" >> ooc_ws2_32_crypt.c
	xxtea /ekey:keys\ws2_pandora.bin /size:8 /ctag:select_s "select" >> ooc_ws2_32_crypt.c
	xxtea /ekey:keys\ws2_pandora.bin /size:8 /ctag:send_s "send" >> ooc_ws2_32_crypt.c
	xxtea /ekey:keys\ws2_pandora.bin /size:8 /ctag:recv_s "recv" >> ooc_ws2_32_crypt.c
	xxtea /ekey:keys\ws2_pandora.bin /size:8 /ctag:htons_s "htons" >> ooc_ws2_32_crypt.c
	xxtea /ekey:keys\ws2_pandora.bin /size:8 /ctag:gethostbyname_s "gethostbyname" >> ooc_ws2_32_crypt.c
	xxtea /ekey:keys\ws2_pandora.bin /size:8 /ctag:getaddrinfo_s "getaddrinfo" >> ooc_ws2_32_crypt.c
	xxtea /ekey:keys\ws2_pandora.bin /size:8 /ctag:freeaddrinfo_s "freeaddrinfo" >> ooc_ws2_32_crypt.c
	xxtea /ekey:keys\ws2_pandora.bin /size:8 /ctag:closesocket_s "closesocket" >> ooc_ws2_32_crypt.c
	xxtea /ekey:keys\ws2_pandora.bin /size:8 /ctag:WSAStartup_s "WSAStartup" >> ooc_ws2_32_crypt.c
	xxtea /ekey:keys\ws2_pandora.bin /size:8 /ctag:WSACleanup_s "WSACleanup" >> ooc_ws2_32_crypt.c
# Objects
ooc.o : ooc\ooc.c include\ooc.h
	tcc -c -o ooc.o ooc\ooc.c $(FLAGS)
ooc_network.o : ooc\ooc_network.c include\ooc_network.h
	tcc -c -o ooc_network.o ooc\ooc_network.c $(FLAGS)
ooc_proxy.o : ooc\ooc_proxy.c include\ooc_proxy.h
	tcc -c -o ooc_proxy.o ooc\ooc_proxy.c $(FLAGS)
ooc_string.o : ooc\ooc_string.c include\ooc_string.h
	tcc -c -o ooc_string.o ooc\ooc_string.c $(FLAGS)
ooc_skipjack.o : ooc\ooc_skipjack.c include\ooc_skipjack.h
	tcc -c -o ooc_skipjack.o ooc\ooc_skipjack.c $(FLAGS)
ooc_resedit.o : ooc\ooc_resedit.c include\ooc_resedit.h
	tcc -c -o ooc_resedit.o ooc\ooc_resedit.c $(FLAGS)
ooc_data.o : ooc\ooc_data.c include\ooc_data.h
	tcc -c -o ooc_data.o ooc\ooc_data.c $(FLAGS)
ooc_hashtable.o : ooc\ooc_hashtable.c include\ooc_hashtable.h
	tcc -c -o ooc_hashtable.o ooc\ooc_hashtable.c $(FLAGS)
ooc_ws2_32.o : ooc_ws2_32_crypt.c ooc\ooc_ws2_32.c include\ooc_ws2_32.h
	tcc -c -o ooc_ws2_32.o ooc\ooc_ws2_32.c $(FLAGS)
pandora_boot.o : pandora_boot.c
	tcc -c -o pandora_boot.o pandora_boot.c $(FLAGS)
# irc test
irc_test.o : irc_test.c
	tcc -c -o irc_test.o irc_test.c $(FLAGS)
# pandora.exe test executable
pandora.o : pandora.c include\pandora.h
	tcc -c -o pandora.o pandora.c $(FLAGS)
# Cleany
clean :
	del $(PRIMATIVES) $(OBJECTS) $(VARIOUS) $(STRINGS)
status :
	@wc -l $(SOURCE_FILES)
xxtea.exe : tools\xxtea-cli.c
	tcc -o xxtea.exe tools\xxtea-cli.c $(FLAGS)
coffeexor-cli.exe: tools\coffeexor-cli.c coffeexor.o
	nasm -felf -o coffeexor.o primatives\coffeexor.asm
	tcc -o coffeexor-cli.exe tools\coffeexor-cli.c coffeexor.o $(FLAGS)
coffee-test.exe : coffee-test.c crypt_call.o
	tcc -o coffee-test.exe coffee-test.c crypt_call.o $(FLAGS)
# Various Test Executables
irc : framework irc_test.o 
	tcc -o irctest.exe irc_test.o $(PRIMATIVES) $(OBJECTS) $(LIBS) $(FLAGS)
pandora : framework pandora.o 
	tcc -o pandora.exe pandora.o $(PRIMATIVES) $(OBJECTS) $(LIBS) $(FLAGS)
