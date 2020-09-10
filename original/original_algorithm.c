#include "./original_algorithm.h"

#include "../decrypt_encrypt_functions.h"

char *read_file(char *file_path) {
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
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

void write_file(char *encrypt, char *decrypt) {
    FILE *fp = fopen("./output/original_algorithm_encrypt.txt", "w");
    FILE *fp2 = fopen("./output/original_algorithm_decrypt.txt", "w");
    if (fp == NULL || fp2 == NULL) {
        exit(EXIT_FAILURE);
    }

    fwrite(encrypt, 1, strlen(encrypt), fp);
    fwrite(decrypt, 1, strlen(decrypt), fp2);
    fclose(fp);
    fclose(fp2);
}

opt_params init_params(char **args, int argc) {
    int opt;
    opt_params input;
    input.print = FALSE;
    input.key = 3;
    while ((opt = getopt(argc, args, "a:k:p")) != -1) {
        switch (opt) {
            case 'a':
                input.text = read_file(optarg);
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

int main(int argc, char *argv[]) {
    opt_params params = init_params(argv, argc);

    char *cript = encrypt_rail_fence(params.key, params.text);

    char *descript = decrypt_rail_fence(params.key, cript);

    if (params.print)
        write_file(cript, descript);
    return 0;
}
