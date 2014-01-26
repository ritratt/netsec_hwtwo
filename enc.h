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
        int retval;
	
	/* Key Derivation using PBKDF2 */
        printf("Enter passphrase\n>");
        fgets(passphrase, 16, stdin);
        gcry_kdf_derive ( (void *) passphrase, strlen(passphrase), GCRY_KDF_PBKDF2, GCRY_MD_SHA512, (void *) salt, strlen(salt), iterations, length, key);
	printf("%d\n", strlen(plaintext));

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
	puts(out);
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
        char *mac = gcry_md_read(hd_md_e, GCRY_MD_SHA512);
        printf("Encrypted mac is %s\n", mac);

        return out;
}

char *dec(char *ciphertext) {
	gcry_cipher_hd_t hd_mac, hd_d;
	gcry_md_hd_t hd_md_d;
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

	err = gcry_cipher_open(&hd_d, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC, 0);
        if(err != 0) {
                printf("Decryption handler not created. Error code %d.\n", err);
                return "1";
        }

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

        err = gcry_cipher_decrypt(hd_d, out, (size_t) 16, ciphertext, (size_t) 16);
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

        gcry_md_write(hd_md_d, ciphertext, strlen(ciphertext));
        char *mac_d = gcry_md_read(hd_md_d, GCRY_MD_SHA512);
        printf("Decrypted mac is %s\n", mac_d);
	printf("%d\n", strlen(out));
	return out;
}



