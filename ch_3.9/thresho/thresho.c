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
#include "fixedptc.h"
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
  fixedpt m0Low, m0High, m1Low, m1High, varLow, varHigh;
  fixedpt prob[NHIST];
  double tempProb,tempm0Low,tempm1Low,tempm0High,tempm1High,tempvarLow,tempvarHigh,tempvarWithin,varWMin;
  fixedpt varWithin;
  fixedpt thresh;
  short invertFlag;             /* if =0, dark -> ON; if =1, dark -> OFF */
  fixedpt nHistM1;
  fixedpt x, y;                    /* image coordinates */
  fixedpt i, j, n;


  if (input (argc, argv, &invertFlag) < 0)
    return (-1);

/* allocate input and output image memory */
  imgI = ImageIn (argv[1]);
  imgIn = ImageGetPtr (imgI);
  height = ImageGetHeight (imgI);
  width = ImageGetWidth (imgI);
  imgO = ImageAlloc (height, width, 8);
  imgOut = ImageGetPtr (imgO);
  printf("height %ld width %ld\n",height,width );

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
  printf("Histogram\n");
  for (i = 0; i < NHIST; i++)
   printf("%ld\t",iHist[i]); 
 printf("Histogram\n");

/* compute probabilities */
 printf("prob\n");
  for (i = 0; i < NHIST; i++){
    tempProb = (double) iHist[i] / (double) n;
     prob[i] = fixedpt_fromfloat(tempProb);
    printf(" %d\t ",tempProb);
    //printf("%f\t",prob[i]);
  }
printf("prob\n");

/* find best threshold by computing moments for all thresholds */
  nHistM1 = NHIST - 1;
  printf("j\tm0Low\t,m1Low\t,m0High,\t,m1High\tvarLow\tvarHigh\tvarWithin\n");
  for (i = 1, thresh = 0, varWMin = 10000000.0; i < nHistM1; i++) {
    m0Low = m0High = m1Low = m1High = varLow = varHigh = 0.0;
    tempm0Low = tempm1Low =tempm0High=tempm1High= tempvarLow = tempvarHigh = tempvarWithin = 0.0;
    for (j = 0; j <= i; j++) {
      tempm0Low = prob[j];
      tempm1Low = j * prob[j];
      m0Low = fixedpt_fromfloat(tempm0Low) ;
      m1Low = fixedpt_fromfloat(tempm1Low);  
    }

    m1Low = (m0Low != 0.0) ? fixedpt_div(m1Low ,m0Low) : i;
     //printf("m0Low:%d\t %f \t ",j,tempm0Low);
   // printf("m0Low:%d\t %f \t ",j,tempm1Low);
    for (j = i + 1; j < NHIST; j++) {
      tempm0High += prob[j];
      tempm1High += j * prob[j];
      m0High = fixedpt_fromfloat(tempm0High) ;
      m1High = fixedpt_fromfloat(tempm1High);

    }
    //printf("m0High:%d\t %f \t ",j,tempm0High);
    //printf("m1High:%d\t %f \t ",j,tempm1High);
    m1High = (m0High != 0.0) ? fixedpt_div(m1High, m0High) : i;
    for (j = 0; j <= i; j++){
      tempvarLow += (j - m1Low) * (j - m1Low) * prob[j];
      varLow = fixedpt_fromfloat(tempvarLow);
     
    }
     //printf("varLow:%d \t %f, \n",j,tempvarLow);
    for (j = i + 1; j < NHIST; j++){
      tempvarHigh += (j - m1High) * (j - m1High) * prob[j];
      varHigh = fixedpt_fromfloat(tempvarHigh);
      //printf("float: %f, fixed point: %s \n",tempvarHigh,fixedpt_cstr(varHigh,-1));
    }

    tempvarWithin = m0Low * varLow + m0High * varHigh;
    printf("%d\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f\n",i,tempm0Low,
      tempm1Low,tempm0High,tempm1High,tempvarLow,tempvarHigh,tempvarWithin);
    varWithin = fixedpt_fromfloat(tempvarWithin);
    //printf("float: %f, fixed point: %s \n",tempvarWithin,fixedpt_cstr(varWithin,-1));
    //printf("%s\t",fixedpt_cstr(varWithin,-1) );
    if (varWithin < varWMin) {
      varWMin = fixedpt_tofloat(varWithin);
      thresh = i;
    }
  }

/*  printf ("Min variance is %5.2f at %d\n", varWMin, thresh); */
  printf ("Calculated threshold value (by Otsu method) = %d\n", thresh);

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
