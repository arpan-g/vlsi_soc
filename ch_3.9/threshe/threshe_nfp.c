/* 
 * threshe.c
 * 
 * Practical Algorithms for Image Analysis
 * 
 * Copyright (c) 1997, 1998, 1999 MLMSoftwareGroup, LLC
 */

/* THRESHE:     program performs binarization by maximum entropy method
 *                    usage: threshe inimg outimg [-i] [-L]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <images.h>
#include <tiffimage.h>
#include <math.h>
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
  double Hn, Ps, Hs;
  double psi, psiMax;
  long thresh;
  short invertFlag;             /* if =0, dark -> ON; if =1, dark -> OFF */
  long x, y;                    /* image coordinates */
  long i, j, n;

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
  printf ("MAIN: Compile histogram...\n");
  for (i = 0; i < NHIST; i++)
    iHist[i] = 0;
  for (y = 0, n = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      iHist[imgIn[y][x]]++;
      n++;
    }
  }

/* compute probabilities */
  for (i = 0; i < NHIST; i++){
    prob[i] = (double) iHist[i] / (double) n;
	
}
	

/* find threshold */
  for (i = 0, Hn = 0.0; i < NHIST; i++)
    if (prob[i] != 0.0){
      Hn -= prob[i] * log (prob[i]);
	printf("%f \t",log (prob[i]));
}

  for (i = 1, psiMax = 0.0; i < NHIST; i++) {
    for (j = 0, Ps = Hs = 0.0; j < i; j++) {
      Ps += prob[j];
      if (prob[j] > 0.0)
        Hs -= prob[j] * log (prob[j]);
    }
    if (Ps > 0.0 && Ps < 1.0)
      psi = log (Ps - Ps * Ps) + Hs / Ps + (Hn - Hs) / (1.0 - Ps);
    if (psi > psiMax) {
      psiMax = psi;
      thresh = i;
    }
  }

  printf ("Calculated threshold value (by Entropy method) = %d.\n", thresh);

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
  printf ("USAGE: threshe inimg outimg [-i] [-L]\n");
  if (flag == 0)
    return (-1);

  printf ("\nthreshe performs binarization with respect to\n");
  printf ("automatically determined intensity threshold;\n");
  printf ("the input gray-level image is converted to a binary image;\n");
  printf ("threshold determination is made by the maximum entropy\n");
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
