#include "server.h"

int main(int argc, char **argv[]) {
	
	if(argc <3 || argc > 4) {
		printf("techdec < filename >  [-d < port >][-l]\n");
		return 2;
	}

	char *filename = argv[1];
	
	if(argc == 3) {
		FILE *fr = fopen(filename, "r");
		char *filename_dec = "";
		strncpy(filename_dec, filename, strlen(filename) - 3);
		puts(filename_dec);
		FILE *fw = fopen(filename_dec, "w+");
		char *ct = calloc(16, sizeof(char *));
		fread(ct, 1, 16, fr);
		fclose(fr);
		char *pt = dec(ct);
		fwrite(pt, 1, strlen(pt), fw);
		fclose(fw);
	}


	if(argc == 4) {
		int port = (int) argv[3];
		server(filename, port);
		return 0;
	}

	return 0;
}

