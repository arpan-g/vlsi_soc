/*
 * threshm.c
 *
 * Practical Algorithms for Image Analysis
 *
 * Copyright (c) 1997, 1998, 1999 MLMSoftwareGroup, LLC
 */

/* THRESHM:     program performs binarization by moment-preservation method
 *                    usage: threshm inimg outimg [-i] [-L]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <images.h>
#include <tiffimage.h>          /* tiff file format info */
#include <math.h>
#include "misc.h"
extern void print_sos_lic ();

#define NHIST 256               /* no. bins in histogram */

long input (int, char **, short *);
long usage (short);

main (
       int argc,
       char *argv[])
{
  Image *imgI, *imgO;           /* I/O image structures */
  unsigned char **imgIn, **imgOut;  /* input and output images */
  long width, height;           /* image size */
  long iHist[NHIST];            /* hist. of intensities */
  double prob[NHIST];
  double pDistr;
  double m1, m2, m3;
  double cd, c0, c1, z0, z1, pd, p0, p1;
  long thresh;
  short invertFlag;             /* if =0, dark -> ON; if =1, dark -> OFF */
  long i, n;
  int i1, j1, i2, j2;
  int c;
  int x1, y1, x2, y2;
  char in_buf[IN_BUF_LEN];

  if ((input (argc, argv, &invertFlag)) < 0)
    return (-1);

/* allocate input and output image memory */
  imgI = ImageIn (argv[1]);
  imgIn = ImageGetPtr (imgI);
  height = ImageGetHeight (imgI);
  width = ImageGetWidth (imgI);

  x1 = y1 = 0;
  x2 = width;
  y2 = height;
  printf ("Specify AOI? (y/n) ");
  c = readlin (in_buf);
  if (c == 'y') {
    while (1) {
      printf ("Input upper left, lower right coordinates");
      if (scanf ("%d %d %d %d", &x1, &y1, &x2, &y2) != 4)
        printf ("Invalid input!  Try again\n");
      else
        break;
    }
  }
  imgO = ImageAlloc (y2 - y1, x2 - x1, 8);
  imgOut = ImageGetPtr (imgO);

/* compile histogram */
  for (i = 0; i < NHIST; i++)
    iHist[i] = 0;
  for (i1 = y1, n = 0; i1 < y2; i1++) {
    for (j1 = x1; j1 < x2; j1++) {
      iHist[imgIn[i1][j1]]++;
      n++;
    }
  }

/* compute probabilities */
  for (i = 0; i < NHIST; i++)
    prob[i] = (double) iHist[i] / (double) n;

/* calculate first 3 moments */
  m1 = m2 = m3 = 0.0;
  for (i = 0; i < NHIST; i++) {
    m1 += i * prob[i];
    m2 += i * i * prob[i];
    m3 += i * i * i * prob[i];
  }
/*  printf ("moments: m1 = %5.2f, m2 = %5.2f, m3 = %5.2f\n", m1, m2, m3); */

  cd = m2 - m1 * m1;
  c0 = (-m2 * m2 + m1 * m3) / cd;
  c1 = (-m3 + m2 * m1) / cd;
  z0 = 0.5 * (-c1 - sqrt (c1 * c1 - 4.0 * c0));
  z1 = 0.5 * (-c1 + sqrt (c1 * c1 - 4.0 * c0));

  pd = z1 - z0;
  p0 = (z1 - m1) / pd;
  p1 = 1.0 - p0;

/* find threshold */
  for (thresh = 0, pDistr = 0.0; thresh < NHIST; thresh++) {
    pDistr += prob[thresh];
    if (pDistr > p0)
      break;
  }

  printf ("Calculated threshold value (by moment method) = %d.\n", thresh);

/* output thresholded image */
  if (invertFlag == 0) {
    for (i1 = y1, i2 = 0, n = 0; i1 < y2; i1++, i2++)
      for (j1 = x1, j2 = 0; j1 < x2; j1++, j2++)
        imgOut[i2][j2] = (imgIn[i1][j1] > thresh) ? 255 : 0;
  }
  else {
    for (i1 = y1, i2 = 0, n = 0; i1 < y2; i1++, i2++)
      for (j1 = x1, j2 = 0; j1 < x2; j1++, j2++)
        imgOut[i2][j2] = (imgIn[i1][j1] < thresh) ? 255 : 0;
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
  printf ("USAGE: threshm inimg outimg [-i] [-L]\n");
  if (flag == 0)
    return (-1);

  printf ("\nthreshm performs binarization with respect to\n");
  printf ("automatically determined intensity threshold;\n");
  printf ("the input gray-level image is converted to a binary image;\n");
  printf ("threshold determination is made by the moment-preservation\n");
  printf ("method.\n\n");
  printf ("ARGUMENTS:\n");
  printf ("    inimg: input image filename (TIF)\n");
  printf ("   outimg: output image filename (TIF)\n\n");
  printf ("OPTIONS:\n");
  printf ("       -i: INVERT: intensities ABOVE (lighter) threshold set to 0\n");
  printf ("           and those BELOW (darker) threshold set to 255\n");
  printf ("       -L: print Software License for this module\n");
  return (-1);
}


/* INPUT:       function reads input parameters
 *                  usage: input (argc, argv)
 */

#define USAGE_EXIT(VALUE) {usage (VALUE); return (-1);}

long
input (argc, argv, invertFlag)
     int argc;
     char *argv[];
     short *invertFlag;         /* if =0, dark -> ON; if =1, dark -> OFF */
{
  long n;

  *invertFlag = 0;

  if (argc == 1)
    USAGE_EXIT (1);
  if (argc == 2)
    USAGE_EXIT (0);

  for (n = 3; n < argc; n++) {
    if (strcmp (argv[n], "-i") == 0) {
      *invertFlag = 1;
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
