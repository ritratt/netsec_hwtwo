#include "client.h"

int main(int argc, char **argv[]) {
	
	char *pt = calloc(16, sizeof(char *));
	char *ct = calloc(16, sizeof(char *));
	
	char *ip_addr = calloc(16, 1);
	char *port = calloc(8, 1);
	if (argc < 3 || argc > 4) {
		printf("Usage: techrypt < input file > [-d < IP-addr:port >][-l ]\n");
		return 2;
	}

	char *filename = argv[1];
	char *mode = argv[2];
	if(argc == 3 && strncmp(mode, "-l", 2) != 0) {
		printf("Unrecognized mode %s.\n Usage techrypt < input file > [-d < IP-addr:port >][-l ]", mode);
		return 2;
	}
	puts("Opening file");
	FILE *fr = fopen(filename, "r");
	puts("File open success!");
	if (fr == NULL) {
		printf("Error opening file. Make sure that the file name is correct and that it exists.\n");
		return 3;
	}
	int buffsize;
	fseek(fr, 0, SEEK_END);
	buffsize = ftell(fr);
	printf("Buffsize in techrypt is %d\n", buffsize);
	rewind(fr);
	fread(pt, 1, buffsize , fr); 
	//fclose(fr);
	if (argc == 3 && strncmp(mode, "-l", 2) == 0) {
		strncat(filename, ".gt", 3);
		if(access(filename, F_OK) != -1) {
			printf("File %s already exists. If you would still like to perform encryption the please delete the existing file before proceeding.\n", filename);
			return 33;
		}
		ct = enc(pt);
		puts(pt);
		
		FILE *fw = fopen(filename, "w+");
		fwrite(ct, 1, strlen(ct), fw);
		fclose(fw);
	}

	
	if (argc == 4) {
		char *ip_addr_raw = argv[3];
		ip_addr = strtok(ip_addr_raw,":");
        	char *port = strtok(NULL,":");
		puts("THIS is how u fared:");
		puts(ip_addr_raw);
		puts(ip_addr);
		puts(port);
		client(filename, buffsize, ip_addr, port);
		return 0;
	}
	return 0;
}
