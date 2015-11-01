/*
 * binarize.c
 *
 * Practical Algorithms for Image Analysis
 *
 * Copyright (c) 1997, 1998, 1999 MLMSoftwareGroup, LLC
 */

/* BINARIZE:    program binarizes image with respect to user-chosen threshold
 *                    usage: binarize inimg outimg [-t THRESH] [-i] [-a] [-L]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tiffimage.h>          /* tiff file format info */
#include <images.h>
#include "misc.h"
extern void print_sos_lic ();

#define HI ((unsigned char) 255)  /* binarization values */
#define LO ((unsigned char) 0)
#define THRESH_DFLT ((unsigned char) 128)  /* default threshold value */

long usage (short);
long input (int, char **, long *, short *, short *);

int argc;
char *argv[];
long *thresh;

main (argc, argv)
     int argc;
     char *argv[];
{
  Image *imgI, *imgO;           /* I/O image structures */
  unsigned char **imgIn, **imgOut;  /* input/output images */
  long width, height;           /* input image size */
  long thresh;                  /* threshold value */
  short invertFlag;             /* if =0, dark -> ON; if =1, dark -> OFF */
  short aoiFlag;                /* if =1, will prompt user for subimage area to binarize */
  int i1, j1, i2, j2;
  int c;
  int x1, y1, x2, y2;
  char in_buf[IN_BUF_LEN];

  if ((input (argc, argv, &thresh, &invertFlag, &aoiFlag)) < 0)
    return (-1);


/* open input and output images */
  imgI = ImageIn (argv[1]);
  imgIn = imgI->img;
  height = ImageGetHeight (imgI);
  width = ImageGetWidth (imgI);

  printf ("image size is %dx%d; threshold = %d\n", width, height, thresh);
  x1 = y1 = 0;
  x2 = width;
  y2 = height;
  if (aoiFlag != 0) {
    printf ("Specify Area of interest? (y/n) ");
    c = readlin (in_buf);
    if (c == 'y') {
      while (1) {
        printf ("Input upper left lower right coordinates: x0 y0 x1 y1\n");
        if (scanf ("%d %d %d %d", &x1, &y1, &x2, &y2) != 4)
          printf ("Invalid input!  Try again\n");
        else
          break;
      }
    }
  }
  imgO = ImageAlloc (y2 - y1, x2 - x1, 8);
  imgOut = ImageGetPtr (imgO);

/* binarize file */
  if (invertFlag == 0) {        /* dark -> ON, light -> OFF */
    for (j1 = y1, j2 = 0; j1 < y2; j1++, j2++)
      for (i1 = x1, i2 = 0; i1 < x2; i1++, i2++)
        if ((long) imgIn[j1][i1] > thresh)
          imgOut[j2][i2] = HI;
        else
          imgOut[j2][i2] = LO;
  }
  else {                        /* dark -> OFF, light -> ON */
    for (j1 = y1, j2 = 0; j1 < y2; j1++, j2++)
      for (i1 = x1, i2 = 0; i1 < x2; i1++, i2++)
        if ((long) imgIn[j1][i1] < thresh)
          imgOut[j2][i2] = HI;
        else
          imgOut[j2][i2] = LO;
  }

  ImageOut (argv[2], imgO);
  return (0);
}



/* USAGE:       function gives instructions on usage of program
 *                    usage: usage (flag)
 *              When flag is 1, the long message is given, 0 gives short.
 */

long
usage (flag)
     short flag;                /* flag =1 for long message; =0 for short message */
{

/* print short usage message or long */
  printf ("USAGE: binarize inimg outimg [-t THRESHOLD] [-i] [-a] [-L]\n");
  if (flag == 0)
    return (-1);

  printf ("\nbinarize applies a user-selected threshold to binarize\n");
  printf ("a gray-scale input image, setting pixel intensities\n");
  printf ("BELOW (darker than) the user-chosen threshold to 0\n");
  printf ("and those ABOVE (lighter than) the threshold to 255\n");
  printf ("resulting in a binary output image.\n\n");
  printf ("ARGUMENTS:\n");
  printf ("         inimg: input image filename (TIF)\n");
  printf ("        outimg: output image filename (TIF)\n\n");
  printf ("OPTIONS:\n");
  printf ("  -t THRESHOLD: gray value between 0 and 255 (or other maximum\n");
  printf ("                intensity value); intensities above THRESHOLD are\n");
  printf ("                set to ON, all others to OFF; default = %d.\n", THRESH_DFLT);
  printf ("            -i: INVERT: intensities ABOVE (lighter) threshold set to 0\n");
  printf ("                and those BELOW (darker) threshold set to 255\n");
  printf ("            -a: Area of Interest Flag - will prompt for subimage to binarize\n");
  printf ("            -L: print Software License for this module\n");
  return (-1);
}


/* INPUT:       function reads input parameters
 *                  usage: input (argc, argv, &thresh)
 */

#define USAGE_EXIT(VALUE) {usage (VALUE); return (-1);}

long
input (argc, argv, thresh, invertFlag, aoiFlag)
     int argc;
     char *argv[];
     long *thresh;              /* threshold value */
     short *invertFlag;         /* if =0, dark -> ON; if =1, dark -> OFF */
     short *aoiFlag;            /* if =1, prompt for subimage area to binarize */
{
  long n;

  *invertFlag = 0;
  *aoiFlag = 0;

  if (argc == 1)
    USAGE_EXIT (1);
  if (argc == 2)
    USAGE_EXIT (0);

  *thresh = THRESH_DFLT;

  for (n = 3; n < argc; n++) {
    if (strcmp (argv[n], "-t") == 0) {
      if (++n == argc || argv[n][0] == '-')
        USAGE_EXIT (0);
      *thresh = atol (argv[n]);
    }
    else if (strcmp (argv[n], "-i") == 0) {
      *invertFlag = 1;
    }
    else if (strcmp (argv[n], "-a") == 0) {
      *aoiFlag = 1;
    }
    else if (strcmp (argv[n], "-L") == 0) {
      print_sos_lic ();
      exit (0);
    }
    else
      USAGE_EXIT (0);
  }

  return (0);
}
