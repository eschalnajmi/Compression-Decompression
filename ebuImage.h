// Defining the data structure ebu and ebc

#ifndef EBUImage_H
#define EBUImage_H

typedef struct _ebuimage
{
    // initialise magic number variables
    unsigned char magicNumber[2];
    unsigned short *magicNumberValue;

    // initialise the dimensions
    int width;
    int height;

    // initialise the array for pixel values
    unsigned char **imageData;
    unsigned char *ptr;

    // initialise other variables
    long numBytes;
    int check;
} ebuImage;

#endif
