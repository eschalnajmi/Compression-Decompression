#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ebuErrors.h"
#include "ebuImage.h"
#include "Converting.h"

#define SUCCESS 0
#define BAD_ARGS 1
#define BAD_FILE 2
#define BAD_MAGIC_NUMBER 3
#define BAD_DIM 4
#define BAD_MALLOC 5
#define BAD_DATA 6
#define BAD_OUTPUT 7
#define MAGIC_NUMBER 0x6365
#define MAX_DIMENSION 262144
#define MIN_DIMENSION 1

int EBCCheckMagicNumber(ebuImage *image, char *path)
{
    if (*image->magicNumberValue != MAGIC_NUMBER)
    {
        // check magic number
        printf("ERROR: Bad Magic Number (%s)\n", path);
        return BAD_MAGIC_NUMBER;
    } // check magic number

    return 100;
}

int EBCCheckBadData(int current, int current2, FILE *sfile, ebuImage *image, char *path, int counter, unsigned char tmp2)
{
    // validate that we have captured 1 pixel value
    if (image->check != 1)
    { // check inputted data
        printf("ERROR: Bad Data (%s)\n", path);
        return BAD_DATA;
    } // check inputted data

    return 100;
}

