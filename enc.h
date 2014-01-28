#include "template.h"

char *enc(char *plaintext);
char *dec(char *ciphertext);

char *enc(char *plaintext) {
	puts("Starting encryption");
	gcry_cipher_hd_t hd, hd_mac;
	gcry_md_hd_t hd_md_e;
	int length_16style = 16 * (strlen(plaintext)/16 + 1);
        char *key = calloc(16, sizeof(char *));
        size_t length = 16;
        const char *IV = "5844";
        char *out = calloc(length_16style, sizeof(char *));
        size_t outsize = length_16style;
        size_t inlen = length_16style;
        char *passphrase = calloc(16, sizeof(char *));
        const char *salt = "NaCl";
        unsigned long iterations = 4096;
        int retval, i = 0;
	
	printf("16style is %d\n", length_16style);
	/* Key Derivation using PBKDF2 */
        printf("Enter passphrase\n>");
        fgets(passphrase, 16, stdin);
        gcry_kdf_derive ( (void *) passphrase, strlen(passphrase), GCRY_KDF_PBKDF2, GCRY_MD_SHA512, (void *) salt, strlen(salt), iterations, length, key);

        /* Encryption phase */
        gcry_error_t err = gcry_cipher_open (&hd, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC, 0);
        if (err != 0) {
                printf("Error code %d.\n", (int) err);
                return "1";
        }
        err = gcry_cipher_setkey (hd, (void *) key, length);
        if (err != 0) {
                printf("Error code %d.\n", (int) err);
                return "1";
        }
        err = gcry_cipher_setiv (hd, IV, length);

        err = gcry_cipher_encrypt (hd, out, outsize, plaintext, inlen);
	printf("Ciphertext is \n");
	for (i = 0; i < strlen(out); i++) {
                printf("%02X", (unsigned char) out[i]);
        }
	printf("\ncount is %d\n", i);

	/* MAC phase */
        err = gcry_md_open(&hd_md_e, GCRY_MD_SHA512, GCRY_MD_FLAG_HMAC);
        if (err != 0) {
                printf("Unable to create MAC handler. Error code %d.\n", (int) err);
                return "1";
        }
        err = gcry_md_setkey ( hd_md_e, (void *) key, length);
        if (err != 0) {
                printf("Unable to create MAC handler. Error code %d.\n", (int) err);
                return "1";
        }
        gcry_md_write(hd_md_e, out, strlen(out));
        char *mac = calloc(64, 1);
	mac = gcry_md_read(hd_md_e, GCRY_MD_SHA512);
	for (i = 0; i < strlen(mac); i++) {
                printf("%02X", (unsigned char) mac[i]);
        }
	printf("\n");
	printf("FYI mac length is %d\n", strlen(mac));
        strcat(out, mac);
	printf("\nAnd the concatenated crap is %s\n", out);
	return out;
}

char *dec(char *ciphertext) {
	gcry_cipher_hd_t hd_mac, hd_d;
	gcry_md_hd_t hd_md_d;
	int length_16style = 16 * (strlen(ciphertext)/16 + 1);
        char *key_d = calloc(16, sizeof(char *));
        size_t length = 16;
        const char *IV = "5844";
        char *out = calloc(length_16style, sizeof(char));
        size_t outsize = length_16style;
        char *passphrase = calloc(16, sizeof(char *));
        const char *salt = "NaCl";
        unsigned long iterations = 4096;
        char *keybuffer;
        int retval, err;
	char *dirty_mac = calloc(64, 1);
	char *ciphertext_macless = malloc(length_16style - 65);
	int i = 0;
	err = gcry_cipher_open(&hd_d, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC, 0);
        if(err != 0) {
                printf("Decryption handler not created. Error code %d.\n", err);
                return "1";
        }

	dirty_mac = strncpy(dirty_mac, ciphertext + strlen(ciphertext) - 64 , 64);
	ciphertext_macless = strncpy(ciphertext_macless, ciphertext, strlen(ciphertext) - 64);
	printf("Raw ct is\n");
	for (i = 0; i < strlen(ciphertext); i++)
		printf("%02X", (unsigned char) ciphertext[i]);
	printf("\n");
	printf("%d\n", i);
	printf("macless ct is\n");
	for (i = 0; i < strlen(ciphertext_macless); i++)
		printf("%02X", (unsigned char) ciphertext_macless[i]);
	printf("\n");
	printf("count is %d\n", i);
	//printf("\nThe concatenated ct that the decryption receives is %s\n", ciphertext);
	//printf("\nThe macless ct is %s\n", ciphertext_macless);
        /* Key Derivation using PBKDF2 */
        printf("Enter passphrase\n>");
        fgets(passphrase, 16, stdin);
        gcry_kdf_derive ( (void *) passphrase, strlen(passphrase), GCRY_KDF_PBKDF2, GCRY_MD_SHA512, (void *) salt, (size_t ) strlen(salt), iterations, length, key_d);

        err = gcry_cipher_setkey(hd_d, (void *) key_d, length);
        if(err != 0) {
                printf("Unable to set decryption key. Error code %d.\n", err);
                return "1";
        }

        err = gcry_cipher_setiv(hd_d, IV, length);
        if(err != 0) {
                printf("Unable to set IV. Error code %d.\n", err);
                return "1";
        }

        err = gcry_cipher_decrypt(hd_d, out, 1024, ciphertext_macless, strlen(ciphertext_macless));
        if(err != 0) {
                printf("Unable to decrypt. Error code %d.\n", err);
                return "1";
	}
	
	/* MAC phase */
	err = gcry_md_open(&hd_md_d, GCRY_MD_SHA512, GCRY_MD_FLAG_HMAC);
        if (err != 0) {
                printf("Unable to create MAC handler. Error code %d.\n", (int) err);
                return "1";
        }
        err = gcry_md_setkey ( hd_md_d, (void *) key_d, length);
        if (err != 0) {
                printf("Unable to create MAC handler. Error code %d.\n", (int) err);
                return "1";
        }
	
	//printf("Calculating mac on %s where key is %s\n", ciphertext_macless, key_d);
        gcry_md_write(hd_md_d, ciphertext_macless, strlen(ciphertext_macless));
        char *mac_d = calloc(64, 1);
	mac_d = gcry_md_read(hd_md_d, GCRY_MD_SHA512);
	printf("Decrypted mac is \n");
	for (i = 0; i < strlen(mac_d); i++) {
		printf("%02X", (unsigned char) mac_d[i]);
	}
	printf("\n");
	printf("i dude %d\n", i);
	printf("FYI dec mac length is %d\n", strlen(mac_d));
	printf("Dirty mac is \n");
	for (i = 0; i < strlen(dirty_mac); i++) {
                printf("%02X", (unsigned char) dirty_mac[i]);
        }
	printf("\n");
	printf("i dude %d\n", i);
	printf("pt is \n%s", out);
	if(strcmp(mac_d, dirty_mac) != 0){
		printf("You're fucked.\n");
	}
	return out;
}

