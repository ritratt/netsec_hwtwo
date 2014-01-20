#include <gcrypt.h>
#include <stdio.h>

int main(int argc, char **argv) {
	
	gcry_cipher_hd_t hd, hd_mac;
	const char *key = malloc(16);
	size_t length = 16;
	const int IV = 5844;
	unsigned char *out = malloc(16);
	size_t outsize = 16;
	const unsigned char *in = "9999999988888888";
	size_t inlen = 16;
	char *passphrase = malloc(16);
	const char *salt = "NaCl";
	unsigned long iterations = 4096;
	char *keybuffer;

	/* Key Derivation using PBKDF2 */
	printf("Enter passphrase\n>");
	fgets(passphrase, 16, stdin);
	gcry_kdf_derive ( (void *) passphrase, sizeof(passphrase), GCRY_KDF_PBKDF2, GCRY_MD_SHA512, (void *) salt, (size_t *) strlen(salt), iterations, length, (void *) key);
	printf("%x\n", (unsigned int) key);

	/* Encryption phase */
	gcry_error_t err = gcry_cipher_open (&hd, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC, 0);
	if (err != 0) {	
		printf("Error code %d.\n", (int) err);
		return 1;
	}
	err = gcry_cipher_setkey (hd, (void *) key, length);
	if (err != 0) {	
		printf("Error code %d.\n", (int) err);
		return 1;
	}
	err = gcry_cipher_setiv (hd, (void *) &IV, length);
	
	err = gcry_cipher_encrypt (hd, out, outsize, in, inlen);
	puts(in);
	printf("Encrypted contents are:\n%x\n", (unsigned int) out);

	/* MAC phase */
	err = gcry_mac_open ( &hd_mac, 0, 0, NULL);
	if (err != 0) {	
		printf("Unable to create MAC handler. Error code %d.\n", (int) err);
		return 1;
	}
	err = gcry_mac_setkey ( hd_mac, (void *) key, sizeof(key));
	if (err != 0) {	
		printf("Unable to create MAC handler. Error code %d.\n", (int) err);
		return 1;
	}
	

	return 0;
}

