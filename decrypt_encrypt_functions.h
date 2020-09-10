#ifndef DECRYPT_ENCRYPT_H
#define DECRYPT_ENCRYPT_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
char *encrypt_rail_fence(int key, char *text);
char *decrypt_rail_fence(int key, char *cipher);
#endif