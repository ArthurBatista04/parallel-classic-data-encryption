
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define TRUE 1
#define FALSE 0

typedef struct opt_params
{
	int num_threads;
	int key;
	char *text;

} opt_params;

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

void writeFile(char *output)
{
	FILE *fp = fopen("output.txt", "w");
	if (fp == NULL)
	{
		exit(EXIT_FAILURE);
	}

	fwrite(output, 1, strlen(output), fp);
	fclose(fp);
}

opt_params init_params(char **args, int argc)
{
	int opt;
	opt_params input;
	while ((opt = getopt(argc, args, "t:a:k:")) != -1)
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
		case '?':
			exit(EXIT_FAILURE);
		default:
			abort();
		}
	}
	return input;
}

int main(int argc, char *argv[])
{
	opt_params params = init_params(argv, argc);

	char *cript = encryptRailFence(params.key, params.text);

	char *descript = decryptRailFence(params.key, cript);

	writeFile(descript);
	return 0;
}
