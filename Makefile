all: techrypt techdec

techrypt: techrypt.c
	gcc -g -o techrypt techrypt.c -lgcrypt
	
techdec: techdec.c
	gcc -g -o techdec techdec.c -lgcrypt

techrypt.c: client.h
	gcc -g client.h

techdec.c: server.h
	gcc -g server.h

client.h: enc.h
	gcc -g enc.h

