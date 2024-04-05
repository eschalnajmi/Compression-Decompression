#include "ebuImage.h"

// declare the functions necessary
void InitBinaryVar(ebuImage *image);

void InitBinaryMagicNum(ebuImage *image, FILE *sFile);

void CreateBinaryArray(ebuImage Image);

int InitBinaryArray(ebuImage *image, FILE *sFile, char *path);