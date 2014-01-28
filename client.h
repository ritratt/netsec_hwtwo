#include "template.h"
#include "enc.h"

int client(char *filename, int buffsize, char *ip_addr, char *port);

int client(char *filename, int buffsize, char *ip_addr, char *port)
{
	int sockfd = 0, n = 0;
	char recvBuff[1024];
	struct sockaddr_in serv_addr; 

	char *pt = calloc(1024, sizeof(char *));
	char *ct = calloc(1024, sizeof(char *));
	
	/* Read plaintext from file */
	FILE *fp;
	fp = fopen(filename, "r+");
	if(fp == NULL) 
	{
		printf("Error opening file. Make sure that it exists in the folder where the encryption buinary is placed.\n");
		return 6;
	}
	fread(pt, sizeof(char *), buffsize + 1, fp);
	fclose(fp);
	ct = enc(pt);
	printf("The client is gonna send the ct as %s\n", ct);
	memset(recvBuff, '0',sizeof(recvBuff));
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Error : Could not create socket \n");
		return 1;
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 
	int port_int = atoi(port);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_int); 

	if(inet_pton(AF_INET, ip_addr, &serv_addr.sin_addr)<=0)
	{
		printf("\n inet_pton error occured\n");
		return 1;
	} 

	if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\n Error : Connect Failed \n");
		return 1;
	} 
	char *length_descr = "lxdsc";
	char *sbuffsize = malloc(8);
	sprintf(sbuffsize, "%d", buffsize);
	/*puts(sbuffsize);
	puts("hmmm");
	//strcat("lxdscr", sbuffsize);
	puts("well");
	//send(sockfd, "lxdsc18", 16, 0);
	close(sockfd);
	wait(1);
	connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));*/
	send(sockfd, ct, strlen(ct), 0); 
	/*while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
	{
		recvBuff[n] = 0;
		if(fputs(recvBuff, stdout) == EOF)
		    printf("\n Error : Fputs error\n");
	} 

	if(n < 0)
		printf("\n Read error \n");
	*/
	return 0;
}
