#ifndef PARALLEL_ALGORITHM_H
#define PARALLEL_ALGORITHM_H
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

extern char **encrypt;

extern char **decrypt;

char *read_file(char *file_path);

char *final_output(char **array_of_chars, int num_of_threads, int text_size);

void write_file(int num_threads, int text_size);

opt_params init_params(char **args, int argc);

char **get_interval_substring(int num_threads, char *text);

void *data_encrypt(void *input);

#endif