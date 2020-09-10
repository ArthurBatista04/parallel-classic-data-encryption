
#include "./decrypt_encrypt_functions.h"
char *
encrypt_rail_fence(int key, char *text) {
    const unsigned long long int text_len = strlen(text);
    // char rail[key][text_len];
    char **rail = malloc(key * sizeof(char *));

    for (int i = 0; i < key; i++) {
        rail[i] = malloc(text_len * sizeof(char));
    }

    for (unsigned long long int i = 0; i < key; i++)
        for (unsigned long long int j = 0; j < text_len; j++)
            rail[i][j] = '0';

    unsigned long long int dir_down = FALSE;
    unsigned long long int row = 0, col = 0;

    for (unsigned long long int i = 0; i < text_len; i++) {
        if (row == 0 || row == key - 1)
            dir_down = !dir_down;

        rail[row][col++] = text[i];

        dir_down ? row++ : row--;
    }

    char *result = (char *)calloc(text_len, sizeof(char));
    unsigned long long int count = 0;
    for (unsigned long long int i = 0; i < key; i++)
        for (unsigned long long int j = 0; j < text_len; j++)
            if (rail[i][j] != '0')
                result[count++] = rail[i][j];

    return result;
}

char *decrypt_rail_fence(int key, char *cipher) {
    const unsigned long long int cipher_len = strlen(cipher);
    // char rail[key][cipher_len];
    char **rail = malloc(key * sizeof(char *));

    for (int i = 0; i < key; i++) {
        rail[i] = malloc(cipher_len * sizeof(char));
    }

    for (unsigned long long int i = 0; i < key; i++)
        for (unsigned long long int j = 0; j < cipher_len; j++)
            rail[i][j] = '0';

    unsigned long long int dir_down;

    unsigned long long int row = 0, col = 0;

    for (unsigned long long int i = 0; i < cipher_len; i++) {
        if (row == 0)
            dir_down = TRUE;
        if (row == key - 1)
            dir_down = FALSE;

        rail[row][col++] = '*';

        dir_down ? row++ : row--;
    }

    unsigned long long int index = 0;
    for (unsigned long long int i = 0; i < key; i++)
        for (unsigned long long int j = 0; j < cipher_len; j++)
            if (rail[i][j] == '*' && index < cipher_len)
                rail[i][j] = cipher[index++];

    char *result = (char *)calloc(cipher_len, sizeof(char));
    unsigned long long int count = 0;
    row = 0, col = 0;
    for (unsigned long long int i = 0; i < cipher_len; i++) {
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