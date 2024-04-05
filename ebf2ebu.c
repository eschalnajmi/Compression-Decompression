#include <stdio.h>
#include <stdlib.h>

#include "Image.h"
#include "ebuImage.h"
#include "Create.h"
#include "ebuCreate.h"
#include "Errors.h"
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
    Image *ebfimage = (Image *)malloc(sizeof(Image));
    FILE *ebfFile = fopen(argv[1], "r");

    ebuImage *ebuimage = (ebuImage *)malloc(sizeof(ebuImage));
    FILE *ebuFile = fopen(argv[2], "w");

    // check the arguments
    int checkarguments = CheckArguments(argc, "ebf2ebu");
    if (checkarguments == SUCCESS)
    {
        return SUCCESS;
    }
    else if (checkarguments == BAD_ARGS)
    {
        return BAD_ARGS;
    }

    // initialise the variables
    InitVar(ebfimage);

    // check file opened successfully
    if (BadFileCheck(ebfFile, argv[1]) == BAD_FILE)
    {
        return BAD_FILE;
    }

    // initialise the magic number
    InitMagicNum(ebfimage, ebfFile);

    // checking against the casted value due to endienness.
    if (CheckMagicNumber(ebfimage, argv[1]) == BAD_MAGIC_NUMBER)
    {
        return BAD_MAGIC_NUMBER;
    }

    // check the dimensions
    if (CheckDimensions(ebfimage, argv[1], ebfFile) == BAD_DIM)
    {
        // close the file as soon as an error is found
        return BAD_DIM;
    }

    // initialise the array
    int returned = InitArray(ebfimage, ebfFile, argv[1]);
    if (returned != 100)
    {
        return returned;
    }

    // create the array
    CreateBinaryArray(*ebuimage);

    // validate that the file has been opened correctly
    if (BadFileCheck(ebuFile, argv[2]) == BAD_FILE)
    {
        return BAD_FILE;
    }

    // check for a bad output
    if (WritetoEBU(ebuFile, ebfFile, ebuimage, ebfimage, argv[2]) == BAD_OUTPUT)
    {
        return BAD_OUTPUT;
    }

    printf("CONVERTED");
    return SUCCESS;
}