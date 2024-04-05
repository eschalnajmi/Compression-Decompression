#include <stdio.h>
#include <stdlib.h>

#include "Image.h"
#include "ebuImage.h"
#include "Converting.h"

#define BAD_OUTPUT 7

int WritetoEBU(FILE *ofile, FILE *ifile, ebuImage *ebuimage, Image *ebfimage, char *path)
{
    // write the header data in one block
    ebuimage->check = fprintf(ofile, "eu\n%d %d\n", ebfimage->height, ebfimage->width);
    // and use the return from fprintf to check that we wrote.
    if (ebuimage->check == 0)
    { // check write
        fclose(ofile);
        free(ebfimage->imageData);
        printf("ERROR: Bad Output (%s)\n", path);
        return BAD_OUTPUT;
    } // check write

    // iterate though the array and print out pixel values
    for (int current = 0; current < ebfimage->height; current++)
    { // writing out
        for (int current2 = 0; current2 < ebfimage->width; current2++)
        {
            ebuimage->check = fwrite(&(ebfimage->imageData[current][current2]), 1, 1, ofile);
            if (ebuimage->check == 0)
            { // check write
                fclose(ofile);
                free(ebfimage->imageData);
                printf("ERROR: Bad Output (%s)\n", path);
                return BAD_OUTPUT;
            } // check write
        }
    }
    return 100;
}

int WritetoEBF(FILE *ofile, FILE *ifile, ebuImage *ebuimage, Image *ebfimage, char *path)
{
    // write the header data in one block
    ebfimage->check = fprintf(ofile, "eb\n%d %d\n", ebuimage->height, ebuimage->width);
    // and use the return from fprintf to check that we wrote.
    if (ebfimage->check == 0)
    { // check write
        fclose(ofile);
        free(ebfimage->imageData);
        printf("ERROR: Bad Output (%s)\n", path);
        return BAD_OUTPUT;
    } // check write

    int counter = 0;

    // iterate though the array and print out pixel values
    for (int current = 0; current < ebuimage->height; current++)
    { // writing out
        for (int current2 = 0; current2 < ebuimage->width; current2++)
        {

            counter++;

            // write to output file
            if (counter == ebuimage->numBytes)
            {
                ebfimage->check = fprintf(ofile, "%u", (ebuimage->imageData[current][current2]));
            }
            else
            {
                ebfimage->check = fprintf(ofile, "%u%c", (ebuimage->imageData[current][current2]), ((current2 + 1) % ebuimage->width) ? ' ' : '\n');
            }

            if (ebfimage->check == 0)
            { // check write
                fclose(ofile);
                free(ebuimage->imageData);
                printf("ERROR: Bad Output (%s)\n", path);
                return BAD_OUTPUT;
            } // check write
        }
    }
    return 100;
}

int WritetoEBC(FILE *ofile, ebuImage *image, char *path)
{
    // write the header data in one block
    image->check = fprintf(ofile, "ec\n%d %d\n", image->height, image->width);

    // and use the return from fprintf to check that we wrote.
    if (image->check == 0)
    { // check write
        fclose(ofile);
        free(image->imageData);
        printf("ERROR: Bad Output (%s)\n", path);
        return BAD_OUTPUT;
    } // check write

    // initialise variables needed
    char compbyte = 0x00000000;
    int compbytelen = 8;
    int shiftby;
    int bitsused = 0;
    int curbytelen = 5;
    int column = 0;

    // iterate through the array
    for (int row = 0; row < image->height; row++)
    {
        column = 0;
        while (column < image->width)
        {
            shiftby = setshiftby(compbytelen, curbytelen);
            bitsused = setbitsused(compbytelen, curbytelen);

            // shift bits from 00011111 to 11111111
            if (compbytelen > 4)
            {
                compbyte = shift(image->imageData[row][column], compbyte, 1, shiftby);
            }
            else
            {
                compbyte = shift(image->imageData[row][column], compbyte, 0, shiftby);
            }

            compbytelen -= bitsused;
            curbytelen -= bitsused;

            if (compbytelen == 0)
            {
                image->check = fwrite(&compbyte, 1, 1, ofile);
                compbytelen = 8;
                compbyte = 0x00000000;
            }

            if (curbytelen == 0)
            {
                curbytelen = 5;
                column++;
            }
        }
    }

    if (image->check == 0)
    { // check write
        fclose(ofile);
        free(image->imageData);
        printf("ERROR: Bad Output (%s)\n", path);
        return BAD_OUTPUT;
    } // check write

    return 100;
}

unsigned char shift(unsigned char currentbyte, unsigned char compressedbyte, int shift, int shiftby)
{
    // shift the bits in the byte
    // shift = 1 is left, shift = 0 is right
    if (shift == 1)
    {
        currentbyte <<= shiftby;
        compressedbyte |= currentbyte;
        return compressedbyte;
    }
    else
    {
        currentbyte >>= shiftby;
        compressedbyte |= currentbyte;
        return compressedbyte;
    }
}

int setshiftby(int compbytelen, int curbytelen)
{
    // calculate the amount to shift by
    int shiftby;

    if (compbytelen > curbytelen)
    {
        shiftby = compbytelen - curbytelen;
        return shiftby;
    }
    else
    {
        shiftby = curbytelen - compbytelen;
        return shiftby;
    }
}

int setbitsused(int compbytelen, int curbytelen)
{
    // calculate the number of bits that are used
    if (compbytelen < curbytelen)
    {
        return compbytelen;
    }
    else
    {
        return curbytelen;
    }
}

int WritetoEBUfromEBC(FILE *ofile, ebuImage *image, char *path)
{
    // write the header data in one block
    image->check = fprintf(ofile, "eu\n%d %d\n", image->height, image->width);

    // check for bad output
    if (image->check < 0)
    {
        return BAD_OUTPUT;
    };

    // iterate through the array and write to output file
    for (int row = 0; row < image->height; row++)
    {
        for (int column = 0; column < image->width; column++)
        {
            image->check = fwrite(&image->imageData[row][column], 1, 1, ofile);

            if (image->check != 1)
            {
                return BAD_OUTPUT;
            };
        };
    };

    return 100;
}