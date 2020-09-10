#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#define TRUE 1
#define FALSE 0

typedef struct opt_params
{
	int num_threads;
	int key;
	char *text;
	int print;

} opt_params;

typedef struct thread_info
{
	int thread_id;
	char *sub_text;
	int key;

} thread_info;

char **encrypt;

char **decrypt;

char *
encryptRailFence(int key, char *text)
{
	const int text_len = strlen(text);
	char rail[key][text_len];

	for (int i = 0; i < key; i++)
		for (int j = 0; j < text_len; j++)
			rail[i][j] = '0';

	int dir_down = FALSE;
	int row = 0, col = 0;

	for (int i = 0; i < text_len; i++)
	{

		if (row == 0 || row == key - 1)
			dir_down = !dir_down;

		rail[row][col++] = text[i];

		dir_down ? row++ : row--;
	}

	char *result = (char *)calloc(text_len, sizeof(char));
	int count = 0;
	for (int i = 0; i < key; i++)
		for (int j = 0; j < text_len; j++)
			if (rail[i][j] != '0')
				result[count++] = rail[i][j];

	return result;
}

char *decryptRailFence(int key, char *cipher)
{
	const int cipher_len = strlen(cipher);
	char rail[key][cipher_len];

	for (int i = 0; i < key; i++)
		for (int j = 0; j < cipher_len; j++)
			rail[i][j] = '0';

	int dir_down;

	int row = 0, col = 0;

	for (int i = 0; i < cipher_len; i++)
	{

		if (row == 0)
			dir_down = TRUE;
		if (row == key - 1)
			dir_down = FALSE;

		rail[row][col++] = '*';

		dir_down ? row++ : row--;
	}

	int index = 0;
	for (int i = 0; i < key; i++)
		for (int j = 0; j < cipher_len; j++)
			if (rail[i][j] == '*' && index < cipher_len)
				rail[i][j] = cipher[index++];

	char *result = (char *)calloc(cipher_len, sizeof(char));
	int count = 0;
	row = 0, col = 0;
	for (int i = 0; i < cipher_len; i++)
	{

		if (row == 0)
			dir_down = TRUE;
		if (row == key - 1)
			dir_down = FALSE;

		if (rail[row][col] != '*')
			result[count++] = rail[row][col++];

		dir_down ? row++ : row--;
	}
	return result;
}
char *readFile(char *file_path)
{
	FILE *fp = fopen(file_path, "r");
	if (fp == NULL)
	{
		printf("File Not Found!\n");
		exit(EXIT_FAILURE);
	}
	fseek(fp, 0L, SEEK_END);
	long int res = ftell(fp);
	char *text = (char *)calloc(res, sizeof(char));
	fseek(fp, 0L, SEEK_SET);
	fread(text, 1, res, fp);
	fclose(fp);
	return text;
}

char *final_output(char **array_of_chars, int num_of_threads, int text_size)
{
	char *output = calloc(text_size, sizeof(char));
	int count = 0;
	for (int i = 0; i < num_of_threads; i++)
	{
		int columns = strlen(array_of_chars[i]);
		for (int j = 0; j < columns; j++)
		{
			output[count++] = array_of_chars[i][j];
		}
	}
	return output;
}

void writeFile(int num_threads, int text_size)
{
	FILE *fp = fopen("output_encrypt.txt", "w");
	FILE *fp2 = fopen("output_decrypt.txt", "w");
	if (fp == NULL || fp2 == NULL)
	{
		exit(EXIT_FAILURE);
	}
	char *encrypt_string = final_output(encrypt, num_threads, text_size);
	char *decrypt_string = final_output(decrypt, num_threads, text_size);

	fwrite(encrypt_string, 1, text_size, fp);
	fclose(fp);
	fwrite(decrypt_string, 1, text_size, fp2);
	fclose(fp2);
}

opt_params init_params(char **args, int argc)
{
	int opt;
	opt_params input;
	input.print = FALSE;
	while ((opt = getopt(argc, args, "t:a:k:p")) != -1)
	{
		switch (opt)
		{
		case 't':
			input.num_threads = strtoul(optarg, NULL, 0);
			break;
		case 'a':
			input.text = readFile(optarg);
			break;
		case 'k':
			input.key = strtoul(optarg, NULL, 0);
			break;
		case 'p':
			input.print = TRUE;
			break;
		case '?':
			exit(EXIT_FAILURE);
		default:
			abort();
		}
	}
	return input;
}

char **getIntervalSubstring(int num_threads, char *text)
{
	int size_text = strlen(text);
	int start, end;
	char **substrings = (char **)calloc(num_threads, sizeof(char *));
	int num_of_substrings = 0;

	int length_per_thread = size_text / num_threads;
	int bonus = size_text - length_per_thread * num_threads;

	for (start = 0, end = length_per_thread;
		 start < size_text;
		 start = end, end = start + length_per_thread)
	{
		if (bonus)
		{
			end++;
			bonus--;
		}
		char *substring = calloc(end - start, sizeof(char));
		int count = 0;
		for (int i = start; i < end; i++)
		{

			substring[count++] = text[i];
		}
		substrings[num_of_substrings++] = substring;
	}

	return substrings;
}

void *data_encrypt(void *input)
{
	thread_info *info = (thread_info *)input;

	encrypt[info->thread_id] = encryptRailFence(info->key, info->sub_text);
	decrypt[info->thread_id] = decryptRailFence(info->key, encrypt[info->thread_id]);
}

int main(int argc, char *argv[])
{
	opt_params params = init_params(argv, argc);
	pthread_t threads[params.num_threads];
	encrypt = (char **)calloc(params.num_threads, sizeof(char *));
	decrypt = (char **)calloc(params.num_threads, sizeof(char *));
	char **substrings = getIntervalSubstring(params.num_threads, params.text);
	int rc;
	for (int t = 0; t < params.num_threads; t++)
	{
		printf("In main: creating thread %ld\n", t + 1);
		thread_info *info = malloc(sizeof(thread_info));
		info->thread_id = t;
		info->key = params.key;

		info->sub_text = substrings[t];
		rc = pthread_create(&threads[t], NULL, data_encrypt, info);
	}

	for (int t = 0; t < params.num_threads; t++)
	{
		rc = pthread_join(threads[t], NULL);
		printf("In main: thread %ld has finished processing\n", t + 1);
	}

	if (params.print)
		writeFile(params.num_threads, strlen(params.text));

	return 0;
}
