#include <stdio.h>
#include <stdlib.h>

#include "Image.h"
#include "ebuImage.h"
#include "Create.h"
#include "ebuCreate.h"
#include "ebuErrors.h"
#include "Converting.h"
#include "CommonErrors.h"

#define SUCCESS 0
#define BAD_ARGS 1
#define BAD_FILE 2
#define BAD_MAGIC_NUMBER 3
#define BAD_DIM 4
#define BAD_MALLOC 5
#define BAD_DATA 6
#define BAD_OUTPUT 7

int main(int argc, char **argv)
{

    // open the files and initialise the images
    ebuImage *ebuimage = (ebuImage *)malloc(sizeof(ebuImage));
    FILE *ebuFile = fopen(argv[1], "r");

    Image *ebfimage = (Image *)malloc(sizeof(Image));
    FILE *ebfFile = fopen(argv[2], "w");

    // check the arguments are valid
    int checkarguments = CheckArguments(argc, "ebu2ebf");
    if (checkarguments == SUCCESS)
    {
        return SUCCESS;
    }
    else if (checkarguments == BAD_ARGS)
    {
        return BAD_ARGS;
    }

    // initialise the variables
    InitBinaryVar(ebuimage);

    // check file opened successfully
    if (BadFileCheck(ebuFile, argv[1]) == BAD_FILE)
    {
        return BAD_FILE;
    }

    // initialise the magic numbers
    InitBinaryMagicNum(ebuimage, ebuFile);

    // checking against the casted value due to endienness.
    if (CheckMagicNumber(ebuimage, argv[1]) == BAD_MAGIC_NUMBER)
    {
        return BAD_MAGIC_NUMBER;
    }

    // check the dimensions
    if (CheckDimensions(ebuimage, argv[1], ebuFile) == BAD_DIM)
    {
        // close the file as soon as an error is found
        return BAD_DIM;
    }

    // initialise the array
    int returned = InitBinaryArray(ebuimage, ebuFile, argv[1]);
    if (returned != 100)
    {
        return returned;
    }

    // create the array
    Create2DArray(*ebfimage);

    // validate that the file has been opened correctly
    if (BadFileCheck(ebfFile, argv[2]) == BAD_FILE)
    {
        return BAD_FILE;
    }

    // check for a bad output
    if (WritetoEBF(ebfFile, ebuFile, ebuimage, ebfimage, argv[2]) == BAD_OUTPUT)
    {
        return BAD_OUTPUT;
    }

    printf("CONVERTED");
    return SUCCESS;
}