/* 
 * threshk.c
 * 
 * Practical Algorithms for Image Analysis
 * 
 * Copyright (c) 1997, 1998, 1999 MLMSoftwareGroup, LLC
 */

/* THRESHK:     program performs binarization by Kittler's method
 *                    usage: threshk in.tif out.tif [-i] [-L]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <images.h>
#include <tiffimage.h>
#include <math.h>
extern void print_sos_lic ();

#define MINCONTRAST_DFLT 10     /* default minimum contrast % of max */
#define MAX_DEV_LEVEL_DFLT 15   /* max. dev. from level % of max */
#define MINRUN_DFLT 1           /* min. run length for it to count */
#define TRANSWID_DFLT 5         /* max. transition width [pixels] */
#define THICK_PARAM_DFLT 80     /* thickness param */
     /* % of threshold range from top */

#define MULTIOUTFLAG_DFLT 0     /* =1 to output multilevel img; or 0 */
#define HISTFLAG_DFLT 1         /* =1 to display hist on imgs; or 0 */
#define PROF_FRACT_THRESH 0.84  /* threshold on sliding profile */

#define BIG 100000000.0
#define NHIST 256               /* no. bins in histogram */
#define NTHRESH 20              /* max no. of threshold ranges */

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
  long x, y;                    /* image coordinates */
  long i, j;

  double m0Low, m0High, m1Low, m1High, varLow, varHigh;
  double term1, term2;
  double prob[NHIST];
  double stdDevLow, stdDevHigh;
  double discr, discrMin, discrMax, discrM1;
  long thresh;
  short invertFlag;             /* if =0, dark -> ON; if =1, dark -> OFF */
  long nHistM1;
  long n;
  double sqrt (), log ();

  if (input (argc, argv, &invertFlag) < 0)
    return (-1);

/* allocate input and output image memory */
  imgI = ImageIn (argv[1]);
  imgIn = ImageGetPtr (imgI);
  height = ImageGetHeight (imgI);
  width = ImageGetWidth (imgI);
  imgO = ImageAlloc (height, width, 8);
  imgOut = ImageGetPtr (imgO);

/* compile histogram */
  for (i = 0; i < NHIST; i++)
    iHist[i] = 0;
  for (y = 0, n = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      iHist[imgIn[y][x]]++;
      n++;
    }
  }

/* compute probabilities */
  for (i = 0; i < NHIST; i++)
    prob[i] = (double) iHist[i] / (double) n;

/* find best threshold by computing discriminant function */
  nHistM1 = NHIST - 1;
  discr = discrM1 = discrMax = discrMin = 0.0;
  for (i = 1, thresh = 0; i < nHistM1; i++) {
    m0Low = m0High = m1Low = m1High = varLow = varHigh = 0.0;
    for (j = 0; j <= i; j++) {
      m0Low += prob[j];
      m1Low += j * prob[j];
    }
    m1Low = (m0Low != 0.0) ? m1Low / m0Low : i;
    for (j = i + 1; j < NHIST; j++) {
      m0High += prob[j];
      m1High += j * prob[j];
    }
    m1High = (m0High != 0.0) ? m1High / m0High : i;
    for (j = 0; j <= i; j++)
      varLow += (j - m1Low) * (j - m1Low) * prob[j];
    stdDevLow = sqrt (varLow);
    for (j = i + 1; j < NHIST; j++)
      varHigh += (j - m1High) * (j - m1High) * prob[j];
    stdDevHigh = sqrt (varHigh);

    if (stdDevLow == 0.0)
      stdDevLow = m0Low;
    if (stdDevHigh == 0.0)
      stdDevHigh = m0High;
    term1 = (m0Low != 0.0) ? m0Low * log (stdDevLow / m0Low) : 0.0;
    term2 = (m0High != 0.0) ? m0High * log (stdDevHigh / m0High) : 0.0;
    discr = term1 + term2;
    if (discr < discrM1)
      discrMin = discr;
    if (discrMin != 0.0 && discr >= discrM1)
      break;
    discrM1 = discr;
  }

  thresh = i;
  printf ("Calculated threshold value (by Kittler/Illingworth method) = %d.\n", thresh);

/* output thresholded image */
  if (invertFlag == 0) {
    for (y = 0, n = 0; y < height; y++)
      for (x = 0; x < width; x++)
        imgOut[y][x] = (imgIn[y][x] > thresh) ? 255 : 0;
  }
  else {
    for (y = 0, n = 0; y < height; y++)
      for (x = 0; x < width; x++)
        imgOut[y][x] = (imgIn[y][x] < thresh) ? 255 : 0;
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
  printf ("USAGE: threshk inimg outimg [-i] [-L]\n");
  if (flag == 0)
    return (-1);

  printf ("\nthreshk performs binarization with respect to\n");
  printf ("automatically determined intensity threshold;\n");
  printf ("the input gray-level image is converted to a binary image;\n");
  printf ("threshold determination is made by Kittler's\n");
  printf ("minimum error method.\n\n");
  printf ("ARGUMENTS:\n");
  printf ("    inimg: input image filename (TIF)\n");
  printf ("   outimg: output image filename (TIF)\n\n");
  printf ("OPTIONS:\n");
  printf ("      -i: INVERT: intensities ABOVE (lighter) threshold set to 0\n");
  printf ("          and those BELOW (darker) threshold set to 255\n");
  printf ("      -L: print Software License for this module\n");

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
