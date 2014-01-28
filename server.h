#include "template.h"
#include "enc.h"

int server(char *filename, int buffsize, int port);

int server(char * filename, int buffsize, int port)
{
	puts("Server bleh");
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr; 
	char *length_desc = calloc(16, 1);
	char *sbuffsize = calloc(8, 1);
	char sendBuff[1024];
	time_t ticks; 
	char *sbuff = malloc(8);
	printf("Received buffsize is %d\n", buffsize);
	char *pt;// = calloc(buffsize + 1, sizeof(char*));
	char *ct;// = calloc(buffsize + 1, sizeof(char*));
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff)); 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	printf("Poor server listening on %d\n", port);
	serv_addr.sin_port = htons(port); 

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
	
	FILE *fw = fopen(filename, "w+");
	
	listen(listenfd, 10); 
//	while(1)
//	{
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
		/*puts("waiting...");
		recv(connfd, length_desc, 8, 0);
		close(connfd);
		puts(length_desc);
		if(strncmp("lxdsc", length_desc, 5) == 0) {
			buffsize = atoi(strncpy(sbuffsize, length_desc + 5, 8));
			printf("%d\n", buffsize);
		}
		else {
			printf("Error receiving length descriptor.\n Cuz I received %s\n", length_desc);
			return "8";
		}*/
		ct = calloc(buffsize + 1, sizeof(char *));
		pt = calloc(buffsize + 1, sizeof(char *));
		recv(connfd, ct, buffsize + 1, 0);
		pt = dec(ct);
		fwrite(pt, 1, strlen(pt), fw);
		fclose(fw);
		ticks = time(NULL);
		snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
		printf("Response to client:\n%.24s\n", ctime(&ticks));
		write(connfd, sendBuff, strlen(sendBuff)); 
		close(connfd);
		sleep(1);
//	}
}
