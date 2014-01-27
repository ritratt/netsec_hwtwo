#include "server.h"

int main(int argc, char **argv[]) {
	

	int buffsize = 0;
	if(argc <3 || argc > 4) {
		printf("techdec < filename >  [-d < port >][-l]\n");
		return 2;
	}

	char *filename = argv[1];
	
	if(argc == 3) {
		FILE *fr = fopen(filename, "r");
		char *filename_dec = "test.dec";
		//strncpy(filename_dec, filename, strlen(filename) - 2);
		puts(filename_dec);
		FILE *fw = fopen(filename_dec, "w+");
		fseek(fr, 0, SEEK_END);
		buffsize = ftell(fr);
		rewind(fr);
		char *ct = calloc(buffsize + 1, sizeof(char *));
		fread(ct, 1, buffsize + 1, fr);
		fclose(fr);
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

