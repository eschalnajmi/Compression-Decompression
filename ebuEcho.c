#include <stdio.h>
#include <stdlib.h>

#include "ebuErrors.h"
#include "ebuImage.h"
#include "ebuCreate.h"
#include "CommonErrors.h"

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

int main(int argc, char **argv)
{ // main

    ebuImage *image = (ebuImage *)malloc(sizeof(ebuImage));
    // open the input file in read mode
    FILE *inputFile = fopen(argv[1], "r");
    // open the output file in write mode
    FILE *outputFile = fopen(argv[2], "w");

    // validate that user has entered 2 arguments (plus the executable name)
    int checkarguments = CheckArguments(argc, "ebuEcho");
    if (checkarguments == SUCCESS)
    {
        return SUCCESS;
    }
    else if (checkarguments == BAD_ARGS)
    {
        return BAD_ARGS;
    }

    // initialise variables needed
    InitBinaryVar(image);

    // check file opened successfully
    if (BadFileCheck(inputFile, argv[1]) == BAD_FILE)
    {
        return BAD_FILE;
    }

    // initialise the magic number
    InitBinaryMagicNum(image, inputFile);

    // checking against the casted value due to endienness.
    if (CheckMagicNumber(image, argv[1]) == BAD_MAGIC_NUMBER)
    {
        return BAD_MAGIC_NUMBER;
    }

    // check the dimensions of the image
    if (CheckDimensions(image, argv[1], inputFile) == BAD_DIM)
    {
        // close the file as soon as an error is found
        return BAD_DIM;
    }

    // initialise the array
    int returned = InitBinaryArray(image, inputFile, argv[1]);
    if (returned != 100)
    {
        return returned;
    }

    // validate that the file has been opened correctly
    if (BadFileCheck(outputFile, argv[2]) == BAD_FILE)
    {
        return BAD_FILE;
    }

    // check for a bad output
    if (CheckBadOutput(outputFile, inputFile, image, argv[2]) == BAD_OUTPUT)
    {
        return BAD_OUTPUT;
    }

    // free allocated memory before exit
    free(image->imageData);
    // close the output file before exit
    fclose(outputFile);

    // print final success message and return
    printf("ECHOED\n");
    return SUCCESS;
} // main()