#! smake
#	$Header: /usr/people/sam/tiff/RCS/Makefile.in,v 1.51 1996/04/29 19:51:20 sam Rel $
#
# Warning, this file was automatically created by the TIFF configure script
#
# Tag Image File Format Library
#
# Copyright (c) 1988-1996 Sam Leffler
# Copyright (c) 1991-1996 Silicon Graphics, Inc.
# 
# Permission to use, copy, modify, distribute, and sell this software and 
# its documentation for any purpose is hereby granted without fee, provided
# that (i) the above copyright notices and this permission notice appear in
# all copies of the software and related documentation, and (ii) the names of
# Sam Leffler and Silicon Graphics may not be used in any advertising or
# publicity relating to the software without the specific, prior written
# permission of Sam Leffler and Silicon Graphics.
# 
# THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
# EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
# WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
# 
# IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
# ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
# OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
# WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
# LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
# OF THIS SOFTWARE.
#

SRCDIR	= .

#
# VERSION:	v3.4033
# DATE:		Sun May 18 16:15:29 EDT 2008
# TARGET:	x86_64-unknown-linux
# CCOMPILER:	/usr/bin/gcc
# CXXCOMPILER:	@CXXCOMPILER@
#

SHELL	= /bin/sh
NULL	=
ECHO	= echo
GENDIST	= ${TOOLROOT}/usr/sbin/gendist

all default:
	@if [ "no" = yes ]; then \
	    ${ECHO} "= "port; cd port; ${MAKE}; \
	else \
	    true; \
	fi
	@${ECHO} "= "libtiff; cd libtiff; ${MAKE}
	@${ECHO} "= "tools; cd tools; ${MAKE}
	@${ECHO} "= "man; cd man; ${MAKE}

install:
	@${ECHO} "= "libtiff; cd libtiff; ${MAKE} install
	@${ECHO} "= "tools; cd tools; ${MAKE} install
	@${ECHO} "= "man; cd man; ${MAKE} install

clean:
	@if [ "no" = yes ]; then \
	    ${ECHO} "= "port; cd port; ${MAKE} clean; \
	else \
	    true; \
	fi
	@${ECHO} "= "libtiff; cd libtiff; ${MAKE} clean
	@${ECHO} "= "tools; cd tools; ${MAKE} clean
	@${ECHO} "= "man; cd man; ${MAKE} clean
#	-cd contrib/dbs; ${MAKE} clean
#	-cd contrib/dbs/xtiff; ${MAKE} clean

clobber distclean: clean
	rm -f Makefile libtiff/port.h config.log
	rm -f libtiff/Makefile
	rm -f tools/Makefile
	rm -f man/Makefile
	rm -f port/Makefile port/install.sh

#
# The folllowing rule creates a binary distribution for IRIX.
#
product::
	test -d dist || mkdir dist
	rm -f dist/rawidb
	SRC=`pwd` RAWIDB=`pwd`/dist/rawidb ${MAKE} install
	rm -f dist/idb
	sort -u +4 dist/rawidb > dist/idb
	${GENDIST} -v -dist dist -idb dist/idb \
	    -sbase `pwd` -spec ${SRCDIR}/dist/tiff.spec

#
# These rules are used to create the source distribution images
#

HOST	= sgi
COMPRESS= gzip
ZIPSUF	= gz

TIFFFILES=\
	configure				\
	config.guess				\
	config.sub				\
	config.site				\
	Makefile.in				\
	README					\
	VERSION					\
	COPYRIGHT				\
	TODO					\
	dist/tiff.spec				\
	    dist/newalpha			\
	    dist/newversion			\
	libtiff/Makefile.in			\
	    libtiff/Makefile.lcc		\
	    libtiff/t4.h			\
	    libtiff/tiff.h			\
	    libtiff/tiffcomp.h			\
	    libtiff/tiffconf.h			\
	    libtiff/tiffio.h			\
	    libtiff/tiffiop.h			\
	    libtiff/mkg3states.c		\
	    libtiff/mkspans.c			\
	    libtiff/mkversion.c			\
	    libtiff/tif_acorn.c			\
	    libtiff/tif_apple.c			\
	    libtiff/tif_atari.c			\
	    libtiff/tif_aux.c			\
	    libtiff/tif_close.c			\
	    libtiff/tif_codec.c			\
	    libtiff/tif_compress.c		\
	    libtiff/tif_dir.h			\
	    libtiff/tif_dir.c			\
	    libtiff/tif_dirinfo.c		\
	    libtiff/tif_dirread.c		\
	    libtiff/tif_dirwrite.c		\
	    libtiff/tif_dumpmode.c		\
	    libtiff/tif_error.c			\
	    libtiff/tif_fax3.c			\
	    libtiff/tif_fax3.h			\
	    libtiff/tif_flush.c			\
	    libtiff/tif_getimage.c		\
	    libtiff/tif_jpeg.c			\
	    libtiff/tif_lzw.c			\
	    libtiff/tif_msdos.c			\
	    libtiff/tif_next.c			\
	    libtiff/tif_open.c			\
	    libtiff/tif_packbits.c		\
	    libtiff/tif_pixarlog.c		\
	    libtiff/tif_predict.h		\
	    libtiff/tif_predict.c		\
	    libtiff/tif_print.c			\
	    libtiff/tif_read.c			\
	    libtiff/tif_strip.c			\
	    libtiff/tif_swab.c			\
	    libtiff/tif_thunder.c		\
	    libtiff/tif_tile.c			\
	    libtiff/tif_unix.c			\
	    libtiff/tif_version.c		\
	    libtiff/tif_vms.c			\
	    libtiff/tif_warning.c		\
	    libtiff/tif_win3.c			\
	    libtiff/tif_win32.c			\
	    libtiff/tif_write.c			\
	    libtiff/tif_zip.c			\
	port/Makefile.in			\
	    port/getopt.c			\
	    port/install.sh.in			\
	    port/irix/so_locations		\
	    port/strcasecmp.c			\
	    port/strtoul.c			\
	tools/Makefile.in			\
	    tools/Makefile.lcc			\
	    tools/fax2tiff.c			\
	    tools/fax2ps.c			\
	    tools/gif2tiff.c			\
	    tools/pal2rgb.c			\
	    tools/ppm2tiff.c			\
	    tools/ras2tiff.c			\
	    tools/rasterfile.h			\
	    tools/rgb2ycbcr.c			\
	    tools/sgi2tiff.c			\
	    tools/sgigt.c			\
	    tools/sgisv.c			\
	    tools/thumbnail.c			\
	    tools/tiff2bw.c			\
	    tools/tiff2ps.c			\
	    tools/tiffcmp.c			\
	    tools/tiffcp.c			\
	    tools/tiffdither.c			\
	    tools/tiffdump.c			\
	    tools/tiffinfo.c			\
	    tools/tiffmedian.c			\
	    tools/tiffsplit.c			\
	    tools/ycbcr.c			\
	man/Makefile.in				\
	    man/fax2tiff.1			\
	    man/fax2ps.1			\
	    man/gif2tiff.1			\
	    man/pal2rgb.1			\
	    man/ppm2tiff.1			\
	    man/ras2tiff.1			\
	    man/rgb2ycbcr.1			\
	    man/sgi2tiff.1			\
	    man/thumbnail.1			\
	    man/tiff2bw.1			\
	    man/tiff2ps.1			\
	    man/tiffcmp.1			\
	    man/tiffcp.1			\
	    man/tiffdither.1			\
	    man/tiffdump.1			\
	    man/tiffgt.1			\
	    man/tiffinfo.1			\
	    man/tiffmedian.1			\
	    man/tiffsplit.1			\
	    man/tiffsv.1			\
	    man/TIFFClose.3t			\
	    man/TIFFError.3t			\
	    man/TIFFFlush.3t			\
	    man/TIFFGetField.3t			\
	    man/TIFFOpen.3t			\
	    man/TIFFPrintDirectory.3t		\
	    man/TIFFReadDirectory.3t		\
	    man/TIFFReadEncodedStrip.3t		\
	    man/TIFFReadEncodedTile.3t		\
	    man/TIFFReadRGBAImage.3t		\
	    man/TIFFReadRawStrip.3t		\
	    man/TIFFReadRawTile.3t		\
	    man/TIFFReadScanline.3t		\
	    man/TIFFReadTile.3t			\
	    man/TIFFRGBAImage.3t		\
	    man/TIFFSetDirectory.3t		\
	    man/TIFFSetField.3t			\
	    man/TIFFWarning.3t			\
	    man/TIFFWriteDirectory.3t 		\
	    man/TIFFWriteEncodedStrip.3t	\
	    man/TIFFWriteEncodedTile.3t		\
	    man/TIFFWriteRawStrip.3t		\
	    man/TIFFWriteRawTile.3t		\
	    man/TIFFWriteScanline.3t		\
	    man/libtiff.3t			\
	    man/TIFFbuffer.3t			\
	    man/TIFFcodec.3t			\
	    man/TIFFmemory.3t			\
	    man/TIFFquery.3t			\
	    man/TIFFsize.3t			\
	    man/TIFFstrip.3t			\
	    man/TIFFswab.3t			\
	    man/TIFFtile.3t			\
	html/Makefile.in			\
	    html/bugs.html			\
	    html/build.html			\
	    html/contrib.html			\
	    html/document.html			\
	    html/images.html			\
	    html/index.html			\
	    html/internals.html			\
	    html/intro.html			\
	    html/libtiff.html			\
	    html/misc.html			\
	    html/support.html			\
	    html/tools.html			\
	    html/v3.4beta033.html		\
	    html/v3.4beta032.html		\
	    html/v3.4beta031.html		\
	    html/v3.4beta029.html		\
	    html/v3.4beta028.html		\
	    html/v3.4beta024.html		\
	    html/v3.4beta018.html		\
	    html/v3.4beta016.html		\
	    html/v3.4beta007.html		\
	${NULL}
OTHERFILES=\
	html/images/back.gif			\
	html/images/bali.jpg			\
	html/images/cat.gif			\
	html/images/cover.jpg			\
	html/images/cramps.gif			\
	html/images/dave.gif			\
	html/images/info.gif			\
	html/images/jello.jpg			\
	html/images/jim.gif			\
	html/images/note.gif			\
	html/images/oxford.gif			\
	html/images/quad.jpg			\
	html/images/ring.gif			\
	html/images/smallliz.jpg		\
	html/images/strike.gif			\
	html/images/warning.gif			\
	${NULL}
CONTRIBFILES=\
	contrib/dbs/README			\
	    contrib/dbs/Imakefile		\
	    contrib/dbs/tiff-bi.c		\
	    contrib/dbs/tiff-grayscale.c	\
	    contrib/dbs/tiff-palette.c		\
	    contrib/dbs/tiff-rgb.c		\
	    contrib/dbs/xtiff/README		\
		contrib/dbs/xtiff/Imakefile	\
		contrib/dbs/xtiff/patchlevel.h	\
		contrib/dbs/xtiff/xtiff.c	\
		contrib/dbs/xtiff/xtifficon.h	\
	contrib/ras/ras2tif.c			\
	contrib/ras/tif2ras.c			\
	contrib/vms/libtiff/makevms.com		\
	    contrib/vms/libtiff/tiff.opt	\
	    contrib/vms/libtiff/tiffshraxp.opt	\
	    contrib/vms/libtiff/tiffshrvax.opt	\
	    contrib/vms/libtiff/tiffvec.mar	\
	    contrib/vms/tools/makevms.com	\
	contrib/tags				\
	contrib/mac-mpw				\
	contrib/acorn				\
	contrib/win32				\
	contrib/win95				\
	contrib/winnt				\
	contrib/mac-cw				\
	contrib/dosdjgpp			\
	${NULL}
DISTFILES=\
	${TIFFFILES}				\
	dist/tiff.alpha				\
	dist/tiff.version			\
	${OTHERFILES}				\
	${CONTRIBFILES}				\
	${NULL}

CONFIG=\
	-with-CC=cc \
	-with-GCOPTS=" " \
	-with-JPEG=yes \
	-with-DIR_JPEG=../src/jpeg-5a \
	-with-ZIP=yes \
	-with-DIR_LIBGZ=../src/zlib \
	${NULL}

rcsclean:
	rcsclean ${TIFFFILES} && co ${TIFFFILES}

alpha:
	(cd ${SRCDIR}/dist; sh newversion)
	-${MAKE} clean
	${MAKE} alpha.stamp
	${SRCDIR}/configure ${CONFIG}
	${MAKE} product
	${MAKE} alpha.tar

# stamp relevant files according to current alpha
alpha.stamp:
	VERSION=`awk '{print "Alpha" $$3}' ${SRCDIR}/dist/tiff.alpha`;\
	NOW=`date`;							\
	for i in ${TIFFFILES}; do					\
	    REV=`rlog -h -d"$$NOW" ${SRCDIR}/$$i|fgrep 'head:'|awk '{print $$2}'`;\
	    rcs "-N$$VERSION:$$REV" "-sExp:$$REV" ${SRCDIR}/$$i && co -sExp ${SRCDIR}/$$i;	\
	done

purge-old-alphas:
	VERSIONS=`awk '{for (i=1; i<=$$3; i++) printf " -nAlpha%03d",i}'\
	    ${SRCDIR}/dist/tiff.alpha`;					\
	for i in ${TIFFFILES}; do					\
	    echo rcs $$VERSIONS ${SRCDIR}/$$i;				\
	    rcs $$VERSIONS ${SRCDIR}/$$i && co ${SRCDIR}/$$i;		\
	done

alphadiff:
	-@for i in ${TIFFFILES}; do					\
	    rcsdiff -r${ALPHA} ${SRCDIR}/$$i;				\
	done

# create alpha distribution archive
alpha.tar:
	VERSION="v`cat ${SRCDIR}/VERSION``awk '{print $$3}' ${SRCDIR}/dist/tiff.alpha`";	\
	rm -f tiff-$$VERSION $$VERSION $$VERSION-tar;			\
	ln -s ${SRCDIR} tiff-$$VERSION;					\
	(for i in ${DISTFILES}; do					\
	   echo $$i;							\
	done) | sed "s;.*;tiff-$$VERSION/&;" >$$VERSION;		\
	tar cvf $$VERSION-tar `cat $$VERSION`;				\
	rm -f tiff-$$VERSION-tar.${ZIPSUF};				\
	cat $$VERSION-tar | ${COMPRESS} >tiff-$$VERSION-tar.${ZIPSUF};	\
	rm -f tiff-$$VERSION $$VERSION $$VERSION-tar;

release:
	(cd ${SRCDIR}/dist; sh newversion)
	-${MAKE} clean
	${MAKE} release.stamp
	${SRCDIR}/configure ${CONFIG}
	${MAKE} product
	${MAKE} release.tar

release.stamp:
	VERSION="Release`sed 's/\./_/g' ${SRCDIR}/VERSION`";		\
	NOW=`date`;							\
	for i in ${TIFFFILES}; do					\
	    REV=`rlog -h -d"$$NOW" ${SRCDIR}/$$i|fgrep 'head:'|awk '{print $$2}'`;\
	    rcs "-N$$VERSION:$$REV" "-sRel:$$REV" ${SRCDIR}/$$i && co -sRel ${SRCDIR}/$$i;	\
	done

# create release distribution archive
release.tar:
	VERSION=v`cat ${SRCDIR}/VERSION`;				\
	rm -f tiff-$$VERSION $$VERSION $$VERSION-tar;			\
	ln -s ${SRCDIR} tiff-$$VERSION;					\
	(for i in ${DISTFILES}; do					\
	   echo $$i;							\
	done) | sed "s;.*;tiff-$$VERSION/&;" >$$VERSION;		\
	tar cvf $$VERSION-tar `cat $$VERSION`;				\
	rm -f tiff-$$VERSION-tar.${ZIPSUF};				\
	cat $$VERSION-tar | ${COMPRESS} >tiff-$$VERSION-tar.${ZIPSUF};	\
	rm -f tiff-$$VERSION $$VERSION $$VERSION-tar;

#
# Create a package of the test images.
#

PICS=\
	pics/README		 \
	pics/cramps.tif		 \
	pics/cramps-tile.tif	\
	pics/fax2d.tif		\
	pics/g3test.tif		\
	pics/jello.tif		\
	pics/jim___cg.tif	\
	pics/jim___dg.tif	\
	pics/jim___gg.tif	\
	pics/jim___ah.tif	\
	pics/strike.tif		\
	pics/oxford.tif		\
	pics/quad-lzw.tif	\
	pics/quad-tile.tif	\
	pics/text.tif		\
	pics/ycbcr-cat.tif	\
	pics/smallliz.tif	\
	pics/zackthecat.tif	\
	pics/fax2d.g3		\
	pics/g3test.g3		\
	${NULL}

pics.tar:
	tar cvf - ${PICS} | ${COMPRESS} > tiffpics.tar.${ZIPSUF}
