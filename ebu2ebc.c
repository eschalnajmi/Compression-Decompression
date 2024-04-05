#include <stdio.h>
#include <stdlib.h>

#include "Converting.h"
#include "Create.h"
#include "ebuCreate.h"
#include "ebcCreate.h"
#include "ebuErrors.h"
#include "ebuImage.h"
#include "ebcErrors.h"
#include "CommonErrors.h"

#define SUCCESS 0
#define BAD_ARGS 1
#define BAD_FILE 2
#define BAD_MAGIC_NUMBER 3
#define BAD_DIM 4
#define BAD_MALLOC 5
#define BAD_DATA 6
#define BAD_OUTPUT 7

int main(int argc, char **argv)
{
  ebuImage *image = (ebuImage *)malloc(sizeof(ebuImage));

  // open files
  FILE *ebuFile = fopen(argv[1], "r");
  FILE *ebcFile = fopen(argv[2], "w");

  // check file opened successfully
  if (BadFileCheck(ebuFile, argv[1]) == BAD_FILE)
  {
    return BAD_FILE;
  }

  // validate that the file has been opened correctly
  if (BadFileCheck(ebcFile, argv[2]) == BAD_FILE)
  {
    return BAD_FILE;
  }

  // check the arguments are valid
  int checkarguments = CheckArguments(argc, "ebu2ebc");
  if (checkarguments == SUCCESS)
  {
    return SUCCESS;
  }
  else if (checkarguments == BAD_ARGS)
  {
    return BAD_ARGS;
  }

  // initialise the variables
  InitBinaryVar(image);
  InitBinaryMagicNum(image, ebuFile);

  // checking against the casted value due to endienness.
  if (CheckMagicNumber(image, argv[1]) == BAD_MAGIC_NUMBER)

  {
    return BAD_MAGIC_NUMBER;
  }

  if (CheckDimensions(image, argv[1], ebuFile) == BAD_DIM)
  {
    // close the file as soon as an error is found
    return BAD_DIM;
  }

  // initialise the array
  int returned = InitBinaryArray(image, ebuFile, argv[1]);
  if (returned != 100)
  {
    return returned;
  }

  // check for a bad output
  if (WritetoEBC(ebcFile, image, argv[2]) == BAD_OUTPUT)
  {
    return BAD_OUTPUT;
  }

  printf("CONVERTED");
  return SUCCESS;
}