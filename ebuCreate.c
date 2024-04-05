#include <stdio.h>
#include <stdlib.h>

#include "ebuErrors.h"
#include "ebuImage.h"
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

void InitBinaryVar(ebuImage *image)
{
    // create a char array to hold magic number
    // and cast to short
    image->magicNumberValue = (unsigned short *)image->magicNumber;

    // create and initialise variables used within code
    image->width = 0;
    image->height = 0;
}

void InitBinaryMagicNum(ebuImage *image, FILE *sFile)
{
    // get first 2 characters which should be magic number
    image->magicNumber[0] = getc(sFile);
    image->magicNumber[1] = getc(sFile);
}

void CreateBinaryArray(ebuImage *image)
{
    // allocate memory for pointers to each row
    image->imageData = (unsigned char **)malloc((image->height) * sizeof(unsigned char *));

    // allocate memory for all data storage simultaneously
    unsigned char *dataBlock = (unsigned char *)malloc((image->height) * (image->width) * sizeof(unsigned char));

    // set up pointers for each row
    for (int row = 0; row < (image->height); row++)
    {
        image->imageData[row] = dataBlock + row * (image->width);
    }
}

int InitBinaryArray(ebuImage *image, FILE *sFile, char *path)
{
    image->numBytes = image->height * image->width;

    CreateBinaryArray(image);

    image->ptr = (unsigned char *)*(image->imageData);

    // if malloc is unsuccessful, it will return a null pointer
    if (CheckMalloc(image) == BAD_MALLOC)
    {
        fclose(sFile);
        return BAD_MALLOC;
    }

    fseek(sFile, 1, SEEK_CUR);

    unsigned char tmp;

    int counter = 0;
    // read in each grey value from the file
    for (int current = 0; current < image->height; current++)
    { // reading in
        for (int current2 = 0; current2 < image->width; current2++)
        {
            image->check = fread(&tmp, 1, 1, sFile);

            if (CheckBadData(current, current2, sFile, image, path, counter, tmp) == BAD_DATA)
            {
                // ensure that allocated data is freed before exit.
                free(image->imageData);
                fclose(sFile);
                return BAD_DATA;
            }

            image->imageData[current][current2] = tmp;
            counter++;
        }
    } // reading in

    // now we have finished using the inputFile we should close it
    fclose(sFile);
    return 100;
}