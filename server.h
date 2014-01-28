#include "template.h"
#include "enc.h"

int server(char *filename, int buffsize, int port);

int server(char * filename, int buffsize, int port)
{
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr; 
	char *length_desc = calloc(16, 1);
	char *sbuffsize = calloc(8, 1);
	char sendBuff[1024];
	time_t ticks; 
	char *sbuff = malloc(8);
	char *pt;// = calloc(buffsize + 1, sizeof(char*));
	char *ct;// = calloc(buffsize + 1, sizeof(char*));
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff)); 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port); 

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
	
	FILE *fw = fopen(filename, "w+");
	
	listen(listenfd, 10); 
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
		ct = calloc(buffsize + 1, sizeof(char *));
		pt = calloc(buffsize + 1, sizeof(char *));
		recv(connfd, ct, buffsize + 1, 0);
		pt = dec(ct);
		fwrite(pt, 1, strlen(pt), fw);
		fclose(fw);
		close(connfd);
		sleep(1);
//	}
}
