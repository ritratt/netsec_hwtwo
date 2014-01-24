#include "template.h"

char *enc(char *filename);
char *dec(char *wtvr);

char *enc(char *plaintext) {
	gcry_cipher_hd_t hd, hd_mac;
	//char *plaintext = calloc(16, sizeof(char *));
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
        int retval;
/*
        FILE *fp = fopen("test.txt", "r+");
        FILE *fw = fopen("test.txt.enc", "w+");
	fread(plaintext, 1, 16, fp);
	fclose(fp);
*/
	
	printf("Encrypting %s\n", plaintext);
	/* Key Derivation using PBKDF2 */
        printf("Enter passphrase\n>");
        fgets(passphrase, 16, stdin);
	printf("Passphrase entered is %s\n", passphrase);
	printf("sizeof is %d, len is %d\n", sizeof(passphrase), strlen(passphrase));
        gcry_kdf_derive ( (void *) passphrase, strlen(passphrase), GCRY_KDF_PBKDF2, GCRY_MD_SHA512, (void *) salt, strlen(salt), iterations, length, key);
        printf("Encryption key is %s\n", key);

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
        printf("Encrypted contents are:\n%x\n", (unsigned int) *out);
        //fwrite(out, 1, 16, fw);
        //fclose(fw);
        return out;
}

char *dec(char *ciphertext) {
	gcry_cipher_hd_t hd_mac, hd_d;
	//char *ciphertext;// = malloc(16);
        char *key_d = calloc(16, sizeof(char *));
        size_t length = 16;
        const char *IV = "5844";
        char *out = calloc(strlen(ciphertext), sizeof(char *));
        size_t outsize = 16;
        size_t inlen = 16;
        char *passphrase = calloc(16, sizeof(char *));
        const char *salt = "NaCl";
        unsigned long iterations = 4096;
        char *keybuffer;
        int retval, err;
	/*FILE *fp;

	//ciphertext = calloc(16, sizeof(char *));
	fp = fopen("test.txt.enc", "r");
        fread(ciphertext, 1, 16, fp);
        fclose(fp);
*/
	err = gcry_cipher_open(&hd_d, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC, 0);
        if(err != 0) {
                printf("Decryption handler not created. Error code %d.\n", err);
                return "1";
        }
	printf("Decrypting\n%s\n", ciphertext);
        /* Key Derivation using PBKDF2 */
        printf("Enter passphrase\n>");
        fgets(passphrase, 16, stdin);
	printf("Passphrase entered is %s\n", passphrase);
        gcry_kdf_derive ( (void *) passphrase, strlen(passphrase), GCRY_KDF_PBKDF2, GCRY_MD_SHA512, (void *) salt, (size_t ) strlen(salt), iterations, length, key_d);
        //printf(" Decryption key is %s\n", key_d);

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

        err = gcry_cipher_decrypt(hd_d, out, (size_t) 16, ciphertext, (size_t) 16);
        if(err != 0) {
                printf("Unable to decrypt. Error code %d.\n", err);
                return "1";
	}

        puts("proper?");
        puts(out);
        /* MAC phase */
        //err = gcry_mac_open ( &hd_mac, 0, 0, NULL);
        if (err != 0) {
                printf("Unable to create MAC handler. Error code %d.\n", (int) err);
                return "1";
        }
        //err = gcry_mac_setkey ( hd_mac, (void *) key, length);
        if (err != 0) {
                printf("Unable to create MAC handler. Error code %d.\n", (int) err);
                return "1";
        }
	return out;
}



