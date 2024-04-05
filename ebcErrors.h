#include "ebuImage.h"

// declare the functions necessary
int EBCCheckMagicNumber(ebuImage *image, char *path);

int EBCCheckBadData(int current, int current2, FILE *sfile, ebuImage *image, char *path, int counter, unsigned char tmp2);

int EBCCheckDifferent(ebuImage *image1, ebuImage *image2);