#
# thresho  Makefile
#
# Practical Algorithms for Image Analysis
#
# Copyright (c) 1997, 1998, 1999 MLMSoftwareGroup, LLC
#

SHELL   = /bin/sh
NULL    =
CC      = /usr/bin/gcc
HOME    = ../../lib
BIN     = $(HOME)/binlin
IPATH   = -I. -I$(HOME)/libtiff/libtiff -I$(HOME)/libimage -I$(HOME)/libip
LPATH   = -L$(HOME)/libtiff/libtiff -L$(HOME)/libimage -L$(HOME)/libip
COPTS   = -g -O

# OPTIONAL DEFINES #################################
DEFOPTS = -DLINUX

# SET CC COMPILE FLAGS #############################
CFLAGS = $(COPTS) $(IPATH) $(DEFOPTS)

CC = /usr/bin/gcc

LIB_TIFF=$(HOME)/libtiff/libtiff
LIB_IMAGE=$(HOME)/libimage
LDFLAGS=$(OPTIONS)

THRESHO = thresho

# EXECUTABLES #######################################

EXECS = $(THRESHO)

# OBJECT FILES #####################################

THRESHOO = \
	thresho.o \
	$(NULL)

# COMPILATION ######################################

all: $(EXECS)

$(THRESHO): $(THRESHOO)
	$(CC) $(COPTS) $(THRESHOO) $(LPATH) -lip -limage -ltiff -lm -o $@
	/bin/cp $@ $(BIN)

# HEADER DEPENDENCIES ###############################
HDRS =  $(HOME)/libimage/tiffimage.h \
	$(HOME)/libimage/draw.h \
	$(HOME)/libip/ip.h \
	$(NULL)

$(THRESHOO):	$(HDRS)

clean:
	rm -f $(EXECS) *.o *.a core

# START OVER ########################################
new:
	rm -f $(EXECS) *.o *.a core
	make

