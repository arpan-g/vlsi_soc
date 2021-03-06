/* 
 * xytoline.c
 * 
 * Practical Algorithms for Image Analysis
 * 
 * Copyright (c) 1997, 1998, 1999 MLMSoftwareGroup, LLC
 *
 *	BUG FIX - 13-Jul-04 [log] re-zeroed endStrt so end segment of a
 *			  LINEBREAK structure will not return to LINEBREAK location
 */

/* XYTOLINE:    function takes input stream of (x,y) coordinates in chains
 *            that may be broken by break features, and orders them through
 *              the breaks.
 *                      usage: xytoline (data, &nData, &nSegments, &nStructs)
 */

#include <stdio.h>
#include <stdlib.h>
#include <images.h>
#include "pcc2.h"               /* header file for PCC programs */

long
xytoline (data, nData, nSegments, nStructs)
     struct point *data;        /* data curve */
     long *nData;               /* no. coords in data curve */
     long *nSegments;           /* no. of segments found */
     long *nStructs;            /* no. of structures found */
{
  struct point *data2;          /* second data curve */
  long nData2;                  /* no. of data coordinates after reorg. */
  long feature;                 /* value of PCC feature */
  long strtBr, endBr;           /* data indices to start, end of line break */
  long endStruct;               /* end index of current structure */
  long iStrt;                   /* starting index into data vector */
  long length, maxLength;       /* maximum length segment completes break */
  long endStrt;                 /* flag = 1 if end joined to start;
                                 * * = 2 if start joined to start; 0 unjoined */
  long iStrtMax, iEndMax;       /* start and end of break completion */
  long endStrtMax;              /* end-to-start joint type of max segment */
  long i, j;

/* initialize second data vector */
  if ((data2 = (struct point *)
       calloc (*nData, sizeof (struct point))) == NULL) {
    printf ("XYTOLINE: not enough memory -- sorry");
    return (-1);
  }
  for (i = 0; i < *nData; i++) {
    data2[i].x = data[i].x;
    data2[i].y = data[i].y;
  }
  nData2 = 0;
  *nSegments = 0;
  *nStructs = 0;

/* extract line segments from data stream */
/* the only complication is that the two segments comprising a LINEBREAK */
/* must be joined up */

  for (i = 0; i < *nData; i++) {
    feature = data2[i].x;       /* first in each segment must be feature */
    if (feature > 0);           /*printf ("ERROR -- expecting feature = %d", feature); */

    else if (feature == -STOPCODE)
      break;

    else if (feature != -LINEBRCODE) {  /* not LINEBRCODE: just copy segment */
      if (feature <= -STARTCODE)
        (*nStructs)++;
      if (data2[i + 1].x >= 0) {  /* non-zero segment length */
        data[nData2++].x = -1;
        (*nSegments)++;
        i++;
        while (data2[i].x >= 0) {
          data[nData2].x = data2[i].x;
          data[nData2++].y = data2[i++].y;
        }
        --i;
      }
    }

    /* LINEBREAK segment -- store start and end, then find second half. */
    /* Second half segment of linebreak is chosen as the segment before */
    /* the start of the next structure, which is the longest segment that */
    /* starts or ends at the starting location of the break. (got that?) */

    else if (feature == -LINEBRCODE) {
      (*nStructs)++;

      /* determine first LINEBR segment */
      endStrt = endStrtMax = 0;
      strtBr = i + 1;
      for (j = strtBr; j < *nData; j++)
        if (data2[j].x < 0)
          break;
      endBr = j - 1;

      /* determine end of structure */
      for (; j < *nData; j++)
        if (data2[j].x <= -STARTCODE)
          break;
      endStruct = j - 1;
      i = endStruct;

      /* find longest segment adjoining LINEBREAK segment */
      length = maxLength = 0;
      iStrt = endBr + 2;
      endStrt = endStrtMax = 0;
      for (j = iStrt; j <= endStruct; j++) {
        if (data2[j].x > 0)
          length++;
        else {
          if (length > 0) {
            if ((ABS (data2[j - 1].x - data2[strtBr].x) <= 1)
			  && (ABS (data2[j - 1].y - data2[strtBr].y) <= 1))
              endStrt = 1;
            else if ((ABS (data2[iStrt].x - data2[strtBr].x) <= 1)
                     && (ABS (data2[iStrt].y - data2[strtBr].y) <= 1))
              endStrt = 2;
            if (endStrt != 0) {
              if (length > maxLength) {  /* choose longest break end */
                maxLength = length;
                iStrtMax = iStrt;
                iEndMax = j - 1;
                endStrtMax = endStrt;
              }
            }
          }
          iStrt = j + 1;
          length = 0;
		  endStrt = 0;	/* log 13-Jul-04 - BUG! - added this so end segment of a
						LINEBREAK structure will not return to LINEBREAK */
        }
      }

      /* copy adjoining segment, either backwards or forwards, then line break */
      data[nData2++].x = -1;
      if (endStrtMax == 1) {
        for (j = iStrtMax; j <= iEndMax; j++) {
          data[nData2].x = data2[j].x;
          data[nData2++].y = data2[j].y;
        }
      }
      else if (endStrtMax == 2) {
        for (j = iEndMax; j >= iStrtMax; --j) {
          data[nData2].x = data2[j].x;
          data[nData2++].y = data2[j].y;
        }
      }
      for (j = strtBr; j <= endBr; j++) {
        data[nData2].x = data2[j].x;
        data[nData2++].y = data2[j].y;
      }
      (*nSegments)++;

      /* copy other segments */
      for (j = endBr + 1; j < endStruct; j++) {
        if (data2[j].x < 0) {
          if (j == (iStrtMax - 1))
            j = iEndMax;
          else {
            data[nData2++].x = -1;
            (*nSegments)++;
            j++;
            while (data2[j].x >= 0) {
              data[nData2].x = data2[j].x;
              data[nData2++].y = data2[j++].y;
            }
            --j;
          }
        }
      }

    }
  }

  if (data[nData2 - 1].x >= 0)
    data[nData2++].x = -1;
  *nData = nData2;

  return (0);
}
