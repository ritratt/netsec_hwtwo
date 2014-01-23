#include "template.h"
#include "enc.h"

int main(int argc, char *argv[])
{
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr; 

	char sendBuff[1025];
	time_t ticks; 
	
	char *pt = malloc(16);
	unsigned char *ct = malloc(16);
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff)); 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000); 

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

	listen(listenfd, 10); 

	while(1)
	{
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
		recv(connfd, ct, 16, 0);
		puts(ct);
		pt = dec("qtvr");
		puts(pt);
		ticks = time(NULL);
		snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
		printf("Response to client:\n%.24s\n", ctime(&ticks));
		write(connfd, sendBuff, strlen(sendBuff)); 

		close(connfd);
		sleep(1);
	}
}
