
#include "./parallel_algorithm.h"

#include "../decrypt_encrypt_functions.h"

char **decrypt;
char **encrypt;

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

void final_output(char **array_of_chars, int num_of_threads, FILE *fp) {
    for (int i = 0; i < num_of_threads; i++) {
        int count = 0;

        long int columns = strlen(array_of_chars[i]);
        char *output = calloc(columns, sizeof(char));
        for (long int j = 0; j < columns; j++) {
            output[count++] = array_of_chars[i][j];
        }
        long int messageSize = columns * sizeof(char);


        fwrite(output, 1, messageSize, fp);
    }
    fclose(fp);
}

void write_file(int num_threads) {
    FILE *fp = fopen("./output/parallel_algorithm_output_encrypt.txt", "w");
    FILE *fp2 = fopen("./output/parallel_algorithm_output_decrypt.txt", "w");
    if (fp == NULL || fp2 == NULL) {
        exit(EXIT_FAILURE);
    }
    final_output(encrypt, num_threads, fp);
    final_output(decrypt, num_threads, fp2);
}

opt_params init_params(char **args, int argc) {
    int opt;
    opt_params input;
    input.print = FALSE;
    input.key = 3;
    while ((opt = getopt(argc, args, "t:a:k:p")) != -1) {
        switch (opt) {
            case 't':
                input.num_threads = strtoul(optarg, NULL, 0);
                break;
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

char **get_interval_substring(int num_threads, char *text) {
    long int size_text = strlen(text);
    long int start, end;
    char **substrings = (char **)calloc(num_threads, sizeof(char *));
    long int num_of_substrings = 0;

    long int length_per_thread = size_text / num_threads;
    long int bonus = size_text - length_per_thread * num_threads;

    for (start = 0, end = length_per_thread;
         start < size_text;
         start = end, end = start + length_per_thread) {
        if (bonus) {
            end++;
            bonus--;
        }
        char *substring = calloc(end - start, sizeof(char));
        long int count = 0;
        for (long int i = start; i < end; i++) {
            substring[count++] = text[i];
        }
        substrings[num_of_substrings++] = substring;
    }

    return substrings;
}

void *data_encrypt(void *input) {
    thread_info *info = (thread_info *)input;

    encrypt[info->thread_id] = encrypt_rail_fence(info->key, info->sub_text);
    decrypt[info->thread_id] = decrypt_rail_fence(info->key, encrypt[info->thread_id]);
}

int main(int argc, char *argv[]) {
    opt_params params = init_params(argv, argc);
    pthread_t threads[params.num_threads];
    encrypt = (char **)calloc(params.num_threads, sizeof(char *));
    decrypt = (char **)calloc(params.num_threads, sizeof(char *));
    char **substrings = get_interval_substring(params.num_threads, params.text);
    int rc;
    for (int t = 0; t < params.num_threads; t++) {
        printf("In main: creating thread %d\n", t + 1);
        thread_info *info = malloc(sizeof(thread_info));
        info->thread_id = t;
        info->key = params.key;

        info->sub_text = substrings[t];
        rc = pthread_create(&threads[t], NULL, data_encrypt, info);
    }

    for (int t = 0; t < params.num_threads; t++) {
        rc = pthread_join(threads[t], NULL);
        printf("In main: thread %d has finished processing\n", t + 1);
    }

    if (params.print)
        write_file(params.num_threads);

    return 0;
}
