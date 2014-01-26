#include <gcrypt.h>
#include <stdio.h>

//char *try(char c);

int main() {
	
	gcry_cipher_hd_t hd, hd_mac, hd_d;
	gcry_md_hd_t hd_md_e, hd_md_d;
	const char *key = malloc(16), *key_d = malloc(16);
	size_t length = 16;
	const char *IV = "5844";
	unsigned char *out = calloc(16, sizeof(char *));
	size_t outsize = 16;
	const unsigned char *in = "9999999988888888";
	size_t inlen = 16;
	char *passphrase = malloc(16);
	const char *salt = "NaCl";
	unsigned long iterations = 4096;
	char *keybuffer;
	int retval;
	FILE *fp = fopen("test.txt", "r+");
	FILE *fw = fopen("test.txt.enc", "w+");
	char *plaintext = malloc(16);
	gcry_error_t err;

	/* Key Derivation using PBKDF2 */
	fread(plaintext, 1, 16, fp); 
	puts(plaintext);
	fclose(fp);

	printf("Enter passphrase\n>");
	fgets(passphrase, 16, stdin);
	gcry_kdf_derive ( (void *) passphrase, sizeof(passphrase), GCRY_KDF_PBKDF2, GCRY_MD_SHA512, (void *) salt, (size_t ) strlen(salt), iterations, length, (void *) key);
	printf("%x\n", (unsigned int) (sizeof(key)));

	/* Encryption phase */
	err = gcry_cipher_open (&hd, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC, 0);
	if (err != 0) {	
		printf("Error code %d.\n", (int) err);
		return 1;
	}
	err = gcry_cipher_setkey (hd, key, length);
	if (err != 0) {	
		printf("Error code %d.\n", (int) err);
		return 1;
	}
	err = gcry_cipher_setiv (hd, (void *) &IV, length);
	
	err = gcry_cipher_encrypt (hd, out, outsize, plaintext, inlen);
	puts(in);
	printf("Encrypted contents are:\n%x\n", (unsigned int) out);
	fwrite(out, 1, 16, fw);
	fclose(fw);
	free(plaintext);
	char *ct = out;
	free(out);
	plaintext = malloc(16);
	fp = fopen("test.txt.enc", "r");
	fread(plaintext, 1, 16, fp);
	fclose(fp);
	puts(plaintext);
	out = malloc(16);

	/* MAC phase */
	err = gcry_md_open(&hd_md_e, GCRY_MD_SHA512, GCRY_MD_FLAG_HMAC);
	if (err != 0) {	
		printf("Unable to create MAC handler. Error code %d.\n", (int) err);
		return 1;
	}
	err = gcry_md_setkey ( hd_md_e, (void *) key, length);
	if (err != 0) {	
		printf("Unable to create MAC handler. Error code %d.\n", (int) err);
		return 1;
	}

	gcry_md_write(hd_md_e, ct, strlen(ct));
	char *mac = gcry_md_read(hd_md_e, GCRY_MD_SHA512);
	printf("Encrypted mac is %x\n", (unsigned int) mac);

	/* Decryption phase */
	err = gcry_cipher_open(&hd_d, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC, 0);
	if(err != 0) {
		printf("Decryption handler not created. Error code %d.\n", err);
		return 1;
	}
	/* Key Derivation using PBKDF2 */
        printf("Enter passphrase\n>");
        fgets(passphrase, 16, stdin);
        gcry_kdf_derive ( (void *) passphrase, sizeof(passphrase), GCRY_KDF_PBKDF2, GCRY_MD_SHA512, (void *) salt, (size_t ) strlen(salt), iterations, length, (void *) key_d);
        printf("%x\n", (unsigned int) key_d);

	err = gcry_cipher_setkey(hd_d, key_d, length);
	if(err != 0) {
		printf("Unable to set decryption key. Error code %d.\n", err);
		return 1;
	}

	err = gcry_cipher_setiv(hd_d, (void *) &IV, length);
	if(err != 0) {
		printf("Unable to set IV. Error code %d.\n", err);
		return 1;
	}
	
	err = gcry_cipher_decrypt(hd_d, out, outsize, plaintext, inlen);
	if(err != 0) {
		printf("Unable to decrypt. Error code %d.\n", err);
		return 1;
	}

	puts("proper?");
	puts(out);
	
	/* MAC phase */
	err = gcry_md_open(&hd_md_d, GCRY_MD_SHA512, GCRY_MD_FLAG_HMAC);
	if (err != 0) {	
		printf("Unable to create MAC handler. Error code %d.\n", (int) err);
		return 1;
	}
	err = gcry_md_setkey ( hd_md_d, (void *) key_d, length);
	if (err != 0) {	
		printf("Unable to create MAC handler. Error code %d.\n", (int) err);
		return 1;
	}

	gcry_md_write(hd_md_d, out, strlen(out));
	char *mac_d = gcry_md_read(hd_md_d, GCRY_MD_SHA512);
	printf("Encrypted mac is %x\n", (unsigned int) mac_d);

	return out;
}

