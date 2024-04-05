#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ebuErrors.h"
#include "ebcErrors.h"
#include "ebuImage.h"
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
#define MAGIC_NUMBER 0x6365
#define MAX_DIMENSION 262144
#define MIN_DIMENSION 1

void EBCInitBinaryVar(ebuImage *image)
{
    // create a char array to hold magic number
    // and cast to short
    image->magicNumberValue = (unsigned short *)image->magicNumber;

    // create and initialise variables used within code
    image->width = 0;
    image->height = 0;
}

void EBCInitBinaryMagicNum(ebuImage *image, FILE *sFile)
{
    // get first 2 characters which should be magic number
    image->magicNumber[0] = getc(sFile);
    image->magicNumber[1] = getc(sFile);
}

void EBCCreateBinaryArray(ebuImage *image)
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

int EBCInitBinaryArray(ebuImage *ebcimage, FILE *ebcFile, char *path)
{

    ebcimage->numBytes = ebcimage->height * ebcimage->width;

    // create the binary array
    EBCCreateBinaryArray(ebcimage);

    ebcimage->ptr = (unsigned char *)*(ebcimage->imageData);

    // if malloc is unsuccessful, it will return a null pointer
    if (CheckMalloc(ebcimage) == BAD_MALLOC)
    {
        fclose(ebcFile);
        return BAD_MALLOC;
    };

    // iterate through the array and set the value in each index to 0
    for (int row = 0; row < ebcimage->height; row++)
    {
        for (int column = 0; column < ebcimage->width; column++)
        {
            ebcimage->imageData[row][column] = 0;
        };
    };

    fseek(ebcFile, 1, SEEK_CUR);

    int pixel_index = 0;

    // iterate through the array
    for (int byte_index = 0; byte_index < ceil((ebcimage->height * ebcimage->width * 5) / 8); byte_index++)
    {

        unsigned char byte = 0;

        ebcimage->check = fread(&byte, 1, 1, ebcFile);

        // calculate indexes needed
        int row = floor((pixel_index) / ebcimage->width);
        int column = ((pixel_index) % ebcimage->width);

        int row_next = floor((pixel_index + 1) / ebcimage->width);
        int column_next = ((pixel_index + 1) % ebcimage->width);

        int row_next_next = floor((pixel_index + 2) / ebcimage->width);
        int column_next_next = ((pixel_index + 2) % ebcimage->width);

        // shift the bits from 11111111 to 00011111
        switch (byte_index % 5)
        {
        case 0:
            ebcimage->imageData[row][column] |= (byte >> 3) & 0b11111;
            ebcimage->imageData[row_next][column_next] |= (byte << 2) & 0b11100;

            pixel_index++;
            break;
        case 1:
            ebcimage->imageData[row][column] |= (byte >> 6) & 0b00011;
            ebcimage->imageData[row_next][column_next] |= (byte >> 1) & 0b11111;
            ebcimage->imageData[row_next_next][column_next_next] |= (byte << 4) & 0b10000;

            pixel_index++;
            pixel_index++;
            break;
        case 2:
            ebcimage->imageData[row][column] |= (byte >> 4) & 0b01111;
            ebcimage->imageData[row_next][column_next] |= (byte << 1) & 0b11110;

            pixel_index++;
            break;
        case 3:
            ebcimage->imageData[row][column] |= (byte >> 7) & 0b00001;
            ebcimage->imageData[row_next][column_next] |= (byte >> 2) & 0b11111;
            ebcimage->imageData[row_next_next][column_next_next] |= (byte << 3) & 0b11000;

            pixel_index++;
            pixel_index++;
            break;
        case 4:
            ebcimage->imageData[row][column] |= (byte >> 5) & 0b00111;
            ebcimage->imageData[row_next][column_next] |= byte & 0b11111;

            pixel_index++;
            pixel_index++;
            break;
        };
    };

    // now we have finished using the inputFile we should close it
    fclose(ebcFile);
    return 100;
}