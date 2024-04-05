#define SUCCESS 0
#define BAD_ARGS 1
#define BAD_FILE 2
#define BAD_MAGIC_NUMBER 3
#define BAD_DIM 4
#define BAD_MALLOC 5
#define BAD_DATA 6
#define BAD_OUTPUT 7
#define MAGIC_NUMBER 0x7565
#define MAX_DIMENSION 262144
#define MIN_DIMENSION 1

#include "ebuImage.h"
#include "CommonErrors.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int CheckArguments(int length, char *prog)
{
    if (length == 1)
    {
        printf("Usage: %s file1 file2", prog);
        return SUCCESS;
    }

    if (length != 3)
    { // check arg count
        printf("ERROR: Bad Arguments\n");
        return BAD_ARGS;
    } // check arg count

    return 100;
}

int BadFileCheck(FILE *sfile, char *path)
{
    if (sfile == NULL)
    { // check file pointer
        printf("ERROR: Bad File Name (%s)\n", path);
        return BAD_FILE;
    } // check file pointer

    return 100;
}