// define data structure for ebf

#ifndef Image_H
#define Image_H

typedef struct _image
{
    // initialise magic number variables
    unsigned char magicNumber[2];
    unsigned short *magicNumberValue;

    // initialise the dimensions
    int width;
    int height;

    // initialise the array for pixel values
    unsigned int **imageData;
    unsigned int *ptr;

    // initialise other variables
    long numBytes;
    int check;
} Image;

#endif