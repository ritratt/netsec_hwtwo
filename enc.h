#include "template.h"

char *enc(char *plaintext);
char *dec(char *ciphertext);

char *enc(char *plaintext) {
	gcry_cipher_hd_t hd, hd_mac;
	gcry_md_hd_t hd_md_e;
        char *key = calloc(16, sizeof(char *));
        size_t length = 16;
        const char *IV = "5844";
        unsigned char *out = calloc(16, sizeof(char *));
        size_t outsize = 16;
        size_t inlen = 16;
        char *passphrase = calloc(16, sizeof(char *));
        const char *salt = "NaCl";
        unsigned long iterations = 4096;
        char *keybuffer;
        int retval, i = 0;
	
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
	printf("Calculating mac on %s where key is %s\n", out, key);
        gcry_md_write(hd_md_e, out, strlen(out));
        char *mac = calloc(64, 1);
	mac = gcry_md_read(hd_md_e, GCRY_MD_SHA512);

	printf("\n%s\n", out);
	printf("\nJust MAC from enc is %s\n", mac);
        strcat(out, mac);
	printf("\nAnd the concatenated crap is %s\n", out);
	return out;
}

char *dec(char *ciphertext) {
	gcry_cipher_hd_t hd_mac, hd_d;
	gcry_md_hd_t hd_md_d;
        char *key_d = calloc(16, sizeof(char *));
        size_t length = 16;
        const char *IV = "5844";
        char *out = calloc(strlen(ciphertext), sizeof(char));
        size_t outsize = strlen(ciphertext);
        char *passphrase = calloc(16, sizeof(char *));
        const char *salt = "NaCl";
        unsigned long iterations = 4096;
        char *keybuffer;
        int retval, err;
	char *dirty_mac = calloc(64, 1);
	char *ciphertext_macless = calloc(strlen(ciphertext) - 64 + 1, 1);
	int i = 0;
	err = gcry_cipher_open(&hd_d, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC, 0);
        if(err != 0) {
                printf("Decryption handler not created. Error code %d.\n", err);
                return "1";
        }

	dirty_mac = strncpy(dirty_mac, ciphertext + strlen(ciphertext) - 64, 64);
	ciphertext_macless = "\0";
	printf("%d\n", strlen(ciphertext));
	ciphertext_macless = strncpy(ciphertext_macless, ciphertext, 1);
	printf("\nThe concatenated ct that the decryption receives is %s\n", ciphertext);
	printf("\nThe macless ct is %s\n", ciphertext_macless);
	for (i = 0; i < strlen(ciphertext_macless); i++) {
                printf("%02X", (unsigned char) ciphertext_macless[i]);
        }
	printf("\n count is %d\n", i);
	printf("\nThe dirty mac is %s\n", dirty_mac);
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

        err = gcry_cipher_decrypt(hd_d, out, outsize, ciphertext_macless, strlen(ciphertext_macless));
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
	
	printf("Calculating mac on %s where key is %s\n", ciphertext_macless, key_d);
        gcry_md_write(hd_md_d, ciphertext_macless, strlen(ciphertext_macless));
        char *mac_d = malloc(64);
	mac_d = gcry_md_read(hd_md_d, GCRY_MD_SHA512);
	for (i = 0; i < strlen(mac_d); i++) {
		printf("%02X", (unsigned char) mac_d[i]);
	}
	printf("\n");
        printf("Decrypted mac is %s\n", mac_d);
	printf("pt is %s", out);
	if(strcmp(mac_d, dirty_mac) != 0){
		printf("You're fucked.\n");
	}
	return out;
}

