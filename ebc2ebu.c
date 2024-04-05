#include <stdio.h>
#include <stdlib.h>

#include "Converting.h"
#include "Create.h"
#include "ebuCreate.h"
#include "ebuErrors.h"
#include "ebuImage.h"
#include "ebcCreate.h"
#include "ebcErrors.h"
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
    // check the arguments are valid
    int argument_valid = CheckArguments(argc, "ebc2ebu");

    if (argument_valid != 100)
    {
        return argument_valid;
    };

    ebuImage *image = (ebuImage *)malloc(sizeof(ebuImage));

    FILE *ebcFile = fopen(argv[1], "r");
    FILE *ebuFile = fopen(argv[2], "w");

    // check file opened successfully
    if (BadFileCheck(ebcFile, argv[1]) == BAD_FILE)
    {
        return BAD_FILE;
    };

    // check file opened successfully
    if (BadFileCheck(ebuFile, argv[2]) == BAD_FILE)
    {
        return BAD_FILE;
    };

    // initialise the variables & magic number
    EBCInitBinaryVar(image);
    EBCInitBinaryMagicNum(image, ebcFile);

    // checking against the casted value due to endienness.
    if (EBCCheckMagicNumber(image, argv[1]) == BAD_MAGIC_NUMBER)
    {
        return BAD_MAGIC_NUMBER;
    }

    // check the dimensions
    if (CheckDimensions(image, argv[1], ebcFile) == BAD_DIM)
    {
        // close the file as soon as an error is found
        return BAD_DIM;
    }

    // initialise the array
    int returned = EBCInitBinaryArray(image, ebcFile, argv[1]);
    if (returned != 100)
    {
        return returned;
    }

    // validate that the file has been opened correctly
    if (BadFileCheck(ebuFile, argv[2]) == BAD_FILE)
    {
        return BAD_FILE;
    }

    // convert to ebu
    WritetoEBUfromEBC(ebuFile, image, argv[1]);

    printf("CONVERTED");
    return SUCCESS;
}