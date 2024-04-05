#include <stdio.h>
#include <stdlib.h>

#include "Errors.h"
#include "Image.h"

#define SUCCESS 0
#define BAD_ARGS 1
#define BAD_FILE 2
#define BAD_MAGIC_NUMBER 3
#define BAD_DIM 4
#define BAD_MALLOC 5
#define BAD_DATA 6
#define BAD_OUTPUT 7
#define MAGIC_NUMBER 0x6265
#define MAX_DIMENSION 262144
#define MIN_DIMENSION 1

int CheckMagicNumber(Image *image, char *path)
{
    if (*image->magicNumberValue != MAGIC_NUMBER)
    {
        // check magic number
        printf("ERROR: Bad Magic Number (%s)\n", path);
        return BAD_MAGIC_NUMBER;
    } // check magic number

    return 100;
}

int CheckDimensions(Image *image, char *path, FILE *sFile)
{

    // scan for the dimensions
    // and capture fscanfs return to ensure we got 2 values.
    image->check = fscanf(sFile, "%d %d", &image->height, &image->width);

    if ((image->check) != 2 || image->height < MIN_DIMENSION || image->width < MIN_DIMENSION || image->height > MAX_DIMENSION || image->width > MAX_DIMENSION)
    { // check dimensions
        // print appropriate error message and return
        fclose(sFile);
        printf("ERROR: Bad Dimensions (%s)\n", path);
        return BAD_DIM;
    } // check dimensions

    return 100;
}

int CheckMalloc(Image *image)
{
    if (image->ptr == NULL)
    { // check malloc
        printf("ERROR: Image Malloc Failed\n");
        return BAD_MALLOC;
    } // check malloc

    return 100;
}

int CheckBadData(int current, int current2, FILE *sfile, Image *image, char *path, int counter)
{
    unsigned int tmp;
    if ((counter == image->numBytes) && (fscanf(sfile, "%u", &tmp) != EOF))
    {
        if (image->check != EOF)
        {
            printf("ERROR: Bad Data (%s)\n", path);
            return BAD_DATA;
        }
    }

    // validate that we have captured 1 pixel value
    if ((image->check) != 1 || (image->imageData[current][current2]) > 31 || (image->imageData[current][current2]) < 0)
    { // check inputted data
        printf("ERROR: Bad Data (%s)\n", path);
        return BAD_DATA;
    } // check inputted data

    return 100;
}

int CheckBadOutput(FILE *ofile, FILE *ifile, Image *image, char *path)
{
    // write the header data in one block
    image->check = fprintf(ofile, "eb\n%d %d\n", image->height, image->width);
    // and use the return from fprintf to check that we wrote.
    if (image->check == 0)
    { // check write
        fclose(ofile);
        free(image->imageData);
        printf("ERROR: Bad Output (%s)\n", path);
        return BAD_OUTPUT;
    } // check write

    int counter = 0;

    // iterate though the array and print out pixel values
    for (int current = 0; current < image->height; current++)
    { // writing out
        for (int current2 = 0; current2 < image->width; current2++)
        {

            counter++;

            // write pixel values into output file
            if (counter == image->numBytes)
            {
                image->check = fprintf(ofile, "%u", (image->imageData[current][current2]));
            }
            else
            {
                image->check = fprintf(ofile, "%u%c", (image->imageData[current][current2]), ((current2 + 1) % image->width) ? ' ' : '\n');
            }

            if (image->check == 0)
            { // check write
                fclose(ofile);
                free(image->imageData);
                printf("ERROR: Bad Output (%s)\n", path);
                return BAD_OUTPUT;
            } // check write
        }
    }
    return 100;
}

int CheckDifferent(Image *image1, Image *image2)
{
    // start with magic number values
    if (*(image1->magicNumberValue) != *(image2->magicNumberValue))
    { // free and exit
        free(image1->imageData);
        free(image2->imageData);
        printf("DIFFERENT\n");
        return SUCCESS;
    } // free and exit

    // check dimensions
    if ((image1->height) != (image2->height) || (image1->width) != (image2->width))
    { // free and exit
        free(image1->imageData);
        free(image2->imageData);
        printf("DIFFERENT\n");
        return SUCCESS;
    } // free and exit

    // and check the pixel values
    for (int n = 0; n < (image1->height); n++)
    {
        for (int n2 = 0; n2 < (image1->width); n2++)
        {
            if ((image1->imageData[n][n2]) != (image2->imageData[n][n2]))
            { // free and exit
                free(image1->imageData);
                free(image2->imageData);
                printf("DIFFERENT\n");
                return SUCCESS;
            } // free and exit
        }
    }
    return 100;
}