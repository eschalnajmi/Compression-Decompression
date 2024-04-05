#include "ebuImage.h"

// declare the functions necessary
int CheckMagicNumber(ebuImage *image, char *path);

int CheckDimensions(ebuImage *image, char *path, FILE *sFile);

int CheckMalloc(ebuImage *image);

int CheckBadData(int current, int current2, FILE *sfile, ebuImage *image, char *path, int counter, unsigned char tmp2);

int CheckBadOutput(FILE *ofile, FILE *ifile, ebuImage *image, char *path);

int CheckDifferent(ebuImage *image1, ebuImage *image2);