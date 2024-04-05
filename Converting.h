#include "Image.h"
#include "ebuImage.h"

// declare the functions necessary
int WritetoEBU(FILE *ofile, FILE *ifile, ebuImage *ebuimage, Image *ebfimage, char *path);

int WritetoEBF(FILE *ofile, FILE *ifile, ebuImage *ebuimage, Image *ebfimage, char *path);

int WritetoEBC(FILE *ofile, ebuImage *image, char *path);

unsigned char shift(unsigned char currentbyte, unsigned char compressedbyte, int shift, int shiftby);

int setshiftby(int compbytelen, int curbytelen);

int setbitsused(int compbytelen, int curbytelen);

int WritetoEBUfromEBC(FILE *ofile, ebuImage *image, char *path);