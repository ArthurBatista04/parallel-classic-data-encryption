#ifndef PARALLEL_ALGORITHM_H
#define PARALLEL_ALGORITHM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define TRUE 1
#define FALSE 0

typedef struct opt_params {
    int num_threads;
    int key;
    char *text;
    int print;

} opt_params;

char *read_file(char *file_path);

void write_file(char *encrypt, char *decrypt);

opt_params init_params(char **args, int argc);
#endif