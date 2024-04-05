#include "ebuImage.h"

// declare the functions necessary
void EBCInitBinaryVar(ebuImage *image);

void EBCInitBinaryMagicNum(ebuImage *image, FILE *sFile);

void EBCCreateBinaryArray(ebuImage Image);

int EBCInitBinaryArray(ebuImage *ebcimage, FILE *ebcFile, char *path);