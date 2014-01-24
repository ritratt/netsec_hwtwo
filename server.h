#include "template.h"
#include "enc.h"

int server();

int server(char * filename, int port)
{
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr; 

	char sendBuff[1025];
	time_t ticks; 
	
	char *pt = calloc(16, sizeof(char*));
	unsigned char *ct = calloc(16, sizeof(char*));
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff)); 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000); 

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

	
	FILE *fw = fopen(filename, "w+");
	
	listen(listenfd, 10); 

	while(1)
	{
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
		recv(connfd, ct, 16, 0);
		pt = dec(ct);
		fwrite(pt, 1, strlen(pt), fw);
		fclose(fw);
		ticks = time(NULL);
		snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
		printf("Response to client:\n%.24s\n", ctime(&ticks));
		write(connfd, sendBuff, strlen(sendBuff)); 

		close(connfd);
		sleep(1);
	}
}
