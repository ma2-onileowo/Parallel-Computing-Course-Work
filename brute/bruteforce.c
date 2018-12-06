	#include <openssl/conf.h>
	#include <openssl/evp.h>
	#include <openssl/err.h>
	#include <string.h>
	#include <time.h>
	#include "omp.h"


	void handleErrors(void)
	{
	ERR_print_errors_fp(stderr);
	abort();
	}

	int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
	unsigned char *iv, unsigned char *ciphertext)
	{
	EVP_CIPHER_CTX *ctx;

	int len;

	int ciphertext_len;

	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
	handleErrors();

	if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
	handleErrors();
	ciphertext_len = len;

	if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
	ciphertext_len += len;

	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
	}


	int main(void)

	{
	int total_thread, thread_id;
	double start_time, end_time;
	start_time = omp_get_wtime();
	printf("Starting of the program, start_time = %f\n", start_time);

	/* A 128 bit key */
	unsigned char *key = (unsigned char *)"secret##########";

	/* A 128 bit IV */
	unsigned char *iv = (unsigned char *)"\x01\x02\x03\x04\x05\x06\x07\x08";

	/* Message to be encrypted */
	unsigned char *plaintext =
			(unsigned char *)"This is a really really top secret!";

	/* Buffer for ciphertext. Ensure the buffer is long enough for the ciphertext which may be longer than the plaintext, dependant on the algorithm and mode */
	unsigned char ciphertext[128];
	unsigned char ciphertextnew[128];

	/* Encrypt the plaintext */
	encrypt (plaintext, strlen ((char *)plaintext), key, iv, ciphertext);

	/* Do something useful with the ciphertext here */
	printf("Ciphertext is:\n");
	BIO_dump_fp (stdout, (const char *)ciphertext, 16);

	char alphabet[] = "ectrapuqs";
	char password_found[17];
	int flag_found=0;

	{
	char password[17] = "################";

	for (int s = 0; s < 9; s++)
	for (int t = 0; t < 9; t++)
	for (int u = 0; u < 9; u++)
		{
		password[0] = alphabet[s];
		password[1] = alphabet[t];
		password[2] = alphabet[u];

			for (int v = 0; v < 9; v++)
			for (int w = 0; w < 9; w++)
			for (int x = 0; x < 9; x++) 
			{
				password[3] = alphabet[v];
				password[4] = alphabet[w];
				password[5] = alphabet[x];

		encrypt(plaintext, strlen((char *)plaintext),
				password, iv, ciphertextnew);

		if (strncmp(ciphertext, ciphertextnew, 16) == 0)
		{
			printf("\n%s", password);
			printf("  Here is the correct key!\n\n");
			
			end_time = omp_get_wtime();

			printf("\nProgram start = %f\n", start_time);
			printf("\nProgram end = %f\n", end_time);
			printf("\n\nProgram runtime = %f seconds\n\n", end_time-start_time);

			exit(0);

			flag_found=1;
			strcpy(password_found,password);
		}

		printf("\n%s", password);

	} // end inner group of loops


	} // end of the outer group of loops
	} // end of parallel region

	// Do something now //
	if (flag_found){
	printf("\nThe password is %s\n",password_found);
	return 0;
	}else{
	printf("Password not found\n");
	return 1;
	}
	}
