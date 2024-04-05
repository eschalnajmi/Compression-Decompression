#include "Image.h"

// declare the functions necessary
int CheckMagicNumber(Image *image, char *path);

int CheckDimensions(Image *image, char *path, FILE *sFile);

int CheckMalloc(Image *image);

int CheckBadData(int current, int current2, FILE *sfile, Image *image, char *path, int counter);

int CheckBadOutput(FILE *ofile, FILE *ifile, Image *image, char *path);

int CheckDifferent(Image *image1, Image *image2);