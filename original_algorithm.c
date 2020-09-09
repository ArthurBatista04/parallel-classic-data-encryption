
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

char *encryptRailFence(char *text, int key)
{
	const int text_len = strlen(text);
	char rail[key][text_len];

	for (int i = 0; i < key; i++)
		for (int j = 0; j < text_len; j++)
			rail[i][j] = '\n';

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
			if (rail[i][j] != '\n')
				result[count++] = rail[i][j];

	return result;
}

char *decryptRailFence(char *cipher, int key)
{
	const int cipher_len = strlen(cipher);
	char rail[key][cipher_len];

	for (int i = 0; i < key; i++)
		for (int j = 0; j < cipher_len; j++)
			rail[i][j] = '\n';

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

int main()
{
	printf("%s\n", encryptRailFence("attack at once", 2));
	printf("%s\n", encryptRailFence("GeeksforGeeks ", 3));
	printf("%s\n", encryptRailFence("defend the east wall", 3));

	printf("%s\n", decryptRailFence("GsGsekfrek eoe", 3));
	printf("%s\n", decryptRailFence("atc toctaka ne", 2));
	printf("%s\n", decryptRailFence("dnhaweedtees alf tl", 3));

	return 0;
}
