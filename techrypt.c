#include "client.h"

int main(int argc, char **argv[]) {
	
	char *pt = calloc(16, sizeof(char *));
	char *ct = calloc(16, sizeof(char *));
	
	if (argc < 3 || argc > 4) {
		printf("Usage: techrypt < input file > [-d < IP-addr:port >][-l ]\n");
		return 2;
	}

	char *filename = argv[1];
	char *ip_addr = argv[3];
	char *mode = argv[2];
	if(argc == 3 && strncmp(mode, "-l", 2) != 0) {
		printf("Unrecognized mode %s.\n Usage techrypt < input file > [-d < IP-addr:port >][-l ]", mode);
		return 2;
	}
	FILE *fr = fopen(filename, "r");
	int buffsize;
	fseek(fr, 0, SEEK_END);
	buffsize = ftell(fr);
	rewind(fr);
	fread(pt, 1, buffsize + 1, fr); 
	fclose(fr);

	if (argc == 3) {
		ct = enc(pt);
		puts(pt);
		strncat(filename, ".gt", 3);
		FILE *fw = fopen(filename, "w+");
		fwrite(ct, 1, strlen(ct), fw);
		fclose(fw);
	}

	if (argc == 4) {
		client(filename, buffsize, ip_addr);
		return 0;
	}
	return 0;
}
