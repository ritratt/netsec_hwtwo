#include "template.h"

char *enc(char *filename);
char *dec(char *wtvr);

char *enc(char *filename) {
	gcry_cipher_hd_t hd, hd_mac;
	char *plaintext = malloc(16);
	puts(plaintext);
        const char *key = malloc(16);
        size_t length = 16;
        const int IV = 5844;
        unsigned char *out = malloc(16);
        size_t outsize = 16;// sizeof(plaintext);
        size_t inlen = 16;
        char *passphrase = malloc(16);
        const char *salt = "NaCl";
        unsigned long iterations = 4096;
        char *keybuffer;
        int retval;
        FILE *fp = fopen("test.txt", "r+");
        FILE *fw = fopen("test.txt.enc", "w+");
	fread(plaintext, 1, 16, fp);
	fclose(fp);
	
	printf("Encrypting %s\n", plaintext);
	/* Key Derivation using PBKDF2 */
        printf("Enter passphrase\n>");
        fgets(passphrase, 16, stdin);
        gcry_kdf_derive ( (void *) passphrase, sizeof(passphrase), GCRY_KDF_PBKDF2, GCRY_MD_SHA512, (void *) salt, (size_t ) strlen(salt), iterations, length, (void *) key);
        printf("Key in hex is %x\n", (unsigned int) key);

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
        err = gcry_cipher_setiv (hd, (void *) &IV, length);

        err = gcry_cipher_encrypt (hd, out, outsize, plaintext, inlen);
        printf("Encrypted contents are:\n%x\n", (unsigned int) *out);
        fwrite(out, 1, 16, fw);
        fclose(fw);
        return out;
}

char *dec(char *wtvr) {
	gcry_cipher_hd_t hd_mac, hd_d;
	char *ciphertext;// = malloc(16);
        const char *key_d = malloc(16);
        size_t length = 16;
        const int IV = 5844;
        unsigned char *out = malloc(16);
        size_t outsize = 16;
        size_t inlen = 16;
        char *passphrase = malloc(16);
        const char *salt = "NaCl";
        unsigned long iterations = 4096;
        char *keybuffer;
        int retval, err;
	FILE *fp;

	ciphertext = malloc(16);
	fp = fopen("test.txt.enc", "r");
        fread(ciphertext, 1, 16, fp);
        fclose(fp);

	err = gcry_cipher_open(&hd_d, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC, 0);
        if(err != 0) {
                printf("Decryption handler not created. Error code %d.\n", err);
                return "1";
        }
	printf("Decrypting\n%s\n", ciphertext);
        /* Key Derivation using PBKDF2 */
        printf("Enter passphrase\n>");
        fgets(passphrase, 16, stdin);
        gcry_kdf_derive ( (void *) passphrase, sizeof(passphrase), GCRY_KDF_PBKDF2, GCRY_MD_SHA512, (void *) salt, (size_t ) strlen(salt), iterations, length, (void *) key_d);
        printf("%x\n", (unsigned int) key_d);

        err = gcry_cipher_setkey(hd_d, (void *) key_d, length);
        if(err != 0) {
                printf("Unable to set decryption key. Error code %d.\n", err);
                return "1";
        }

        err = gcry_cipher_setiv(hd_d, (void *) &IV, length);
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



