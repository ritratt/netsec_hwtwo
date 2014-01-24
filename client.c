#include "template.h"
#include "enc.h"

int main(int argc, char *argv[])
{
	int sockfd = 0, n = 0;
	char recvBuff[1024];
	struct sockaddr_in serv_addr; 

	char *pt = calloc(16, sizeof(char *));
	char *ct = calloc(16, sizeof(char *));
	
	/* Read plaintext from file */
	FILE *fp;
	fp = fopen("test.txt", "r+");
	fread(pt, sizeof(char *), 16, fp);
	fclose(fp);
	puts(pt);
	ct = enc(pt);
	puts(ct);
	
        if(argc != 5)
	{
		printf("\n Usage: techrypt < input file > [-d < IP-addr:port >][-l ] \n");
		return 1;
	} 
	filename = argv[1];

	memset(recvBuff, '0',sizeof(recvBuff));
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Error : Could not create socket \n");
		return 1;
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000); 

	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
	{
		printf("\n inet_pton error occured\n");
		return 1;
	} 

	if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\n Error : Connect Failed \n");
		return 1;
	} 

	printf("crap\n");
	send(sockfd, ct, strlen(ct), 0); 
	while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
	{
		recvBuff[n] = 0;
		if(fputs(recvBuff, stdout) == EOF)
		    printf("\n Error : Fputs error\n");
	} 

	if(n < 0)
		printf("\n Read error \n");

	return 0;
}
