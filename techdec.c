#include "server.h"

int main(int argc, char **argv[]) {
	
	int buffsize = 0;
	if(argc <3 || argc > 4) {
		printf("techdec < filename >  [-d < port >][-l]\n");
		return 2;
	}

	char *filename = argv[1];
	char *mode = argv[2];
	
	if(argc == 3 && strncmp(mode, "-l", 2) != 0) {
		printf("Unrecognized mode %s. Usage techdec < filename > [-d <port>][-l]\n", mode);
		return 2;
	}

	if(argc == 3 && strncmp(mode, "-l", 2) == 0) {
		FILE *fr = fopen(filename, "r");
		char *filename_dec = malloc(16);
		strncpy(filename_dec, filename, strlen(filename) - 3);
		puts(filename_dec);
		
		/* Check if a file with decrypted filename already exists. */
		if(access(filename_dec, F_OK) != -1) {
			printf("Decrypted file %s already exists. If you would still like to perform decryption then please delete the existing file before proceeding.\n", filename_dec);
			return 33;
		}
		FILE *fw = fopen(filename_dec, "w+");
		fseek(fr, 0, SEEK_END);
		buffsize = ftell(fr);
		printf("Buffsize is %d\n", buffsize);
		rewind(fr);
		char *ct = calloc(buffsize + 1, sizeof(char));
		fread(ct, 1, buffsize + 1, fr);
		fclose(fr);
		puts(ct);
		char *pt = dec(ct);
		fwrite(pt, 1, strlen(pt), fw);
		fclose(fw);
	}

	if(argc == 4) {
		int port = (int) argv[3];
		server(filename, 64, port);
		return 0;
	}

	return 0;
}

