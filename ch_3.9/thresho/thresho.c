/* 
 * thresho.c
 * 
 * Practical Algorithms for Image Analysis
 * 
 * Copyright (c) 1997, 1998, 1999 MLMSoftwareGroup, LLC
 */

/* THRESHO:     program performs binarization by Otsu's discriminant method
 *                    usage: thresho inimg outimg [-i] [-L]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <images.h>
#include <tiffimage.h>
#include <math.h>
#include <string.h>
extern void print_sos_lic ();


#define NHIST 256               /* no. bins in histogram */

long usage (short);
long input (int, char **, short *);

main (int argc, char *argv[])
{
  Image *imgI, *imgO;           /* I/O image structures */
  unsigned char **imgIn, **imgOut;  /* input and output images */
  long width, height;           /* image size */
  long iHist[NHIST];            /* hist. of intensities */
  int32_t m0Low, m0High, m1Low, m1High, varLow, varHigh;
  int32_t prob[NHIST];
  int32_t varWithin, varWMin;
  int32_t thresh;
  // int64_t m0Low, m0High, m1Low, m1High, varLow, varHigh;
  // int64_t prob[NHIST];
  // int64_t varWithin, varWMin;
  // int64_t thresh;
  short invertFlag;             /* if =0, dark -> ON; if =1, dark -> OFF */
  long nHistM1;
  long x, y;                    /* image coordinates */
  long i, j, n;
  int32_t scale =14;
  int32_t ad = 2;

  if (input (argc, argv, &invertFlag) < 0)
    return (-1);
   //scale = strtol(argv[2], NULL, 0);
   printf("%d\n",scale );
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
    prob[i] =  (iHist[i]<<scale) / n;
    
  }

/* find best threshold by computing moments for all thresholds */
  nHistM1 = NHIST - 1;
  for (i = 1, thresh = 0, varWMin = 0x7fffffff; i < nHistM1; i++) {
    m0Low = m0High = m1Low = m1High = varLow = varHigh = 0;
    for (j = 0; j <= i; j++) {
      m0Low += prob[j];
      m1Low += j * prob[j];
    }

    m1Low = (m0Low != 0) ? (m1Low<<ad) / m0Low : i<<ad;
    //printf("%d %d\n",m1Low,i);
    for (j = i + 1; j < NHIST; j++) {
      m0High += prob[j];
      m1High += j * prob[j];
    }
    m1High = (m0High != 0) ? (m1High<<ad) / m0High : i<<ad;
     
    // printf("new \n");
    for (j = 0; j <= i; j++){
     varLow += ((((j<<ad) - m1Low) * ((j<<ad) - m1Low) * prob[j])>>scale);
      
    }
    for (j = i + 1; j < NHIST; j++){
      varHigh += ((((j<<ad) - m1High) * ((j<<ad) - m1High) * prob[j])>>scale);
    }
     //printf("%d\t%d\t%d\n",m0High,m1High,i);
     //printf("%d\t%d\t%d\n",m0Low,m1Low,i);
     //printf("%d\t%d\t%d\n",varLow,varHigh,i);
     varWithin = m0Low * varLow + m0High * varHigh;
    printf("%d\t%d\t%d\n",varWMin,varHigh,varWithin);
    if (varWithin < varWMin) {
      varWMin = varWithin;
      thresh = i;
    }
  }

/*  printf ("Min variance is %5.2f at %d\n", varWMin, thresh); */
  printf ("Calculated threshold value (by Otsu method) = %d\t", thresh);

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

  ImageOut ("out.pgm", imgO);
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
  printf ("USAGE: thresho inimg outimg [-i] [-L]\n");
  if (flag == 0)
    exit (1);

  printf ("\nthresho program performs binarization with respect to\n");
  printf ("automatically determined intensity threshold;\n");
  printf ("the input gray-level image is converted to a binary image;\n");
  printf ("threshold determination is made by Otsu's\n");
  printf ("moment preservation method.\n\n");
  printf ("ARGUMENTS:\n");
  printf ("    inimg: input image filename (TIF)\n");
  printf ("   outimg: output image filename (TIF)\n\n");
  printf ("OPTIONS:\n");
  printf ("       -i: INVERT: intensities ABOVE (lighter) threshold set to 0\n");
  printf ("           and those BELOW (darker) threshold set to 255\n");
  printf ("       -L: print Software License for this module\n");

  return (-1);
}


#define USAGE_EXIT(VALUE) {usage (VALUE); return (-1);}

/* INPUT:       function reads input parameters
 *                  usage: input (argc, argv)
 */

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
