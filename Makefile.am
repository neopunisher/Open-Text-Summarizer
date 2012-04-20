
SUBDIRS=src dic doc
DIST_SUBDIRS=src articles dic doc

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libots-1.pc

EXTRA_DIST = 		\
    libots-1.pc.in	\
    ots.spec.in		\
    ots.spec		\
    README TODO HACKING NEWS BUGS ChangeLog COPYING

DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc

MAINTAINERCLEANFILES = \
	$(srcdir)/INSTALL \
	$(srcdir)/aclocal.m4 \
	$(srcdir)/autoscan.log \
	$(srcdir)/compile \
	$(srcdir)/config.guess \
	$(srcdir)/config.h.in \
	$(srcdir)/config.sub \
	$(srcdir)/configure \
	$(srcdir)/configure.scan \
	$(srcdir)/depcomp \
	$(srcdir)/install-sh \
	$(srcdir)/ltmain.sh \
	$(srcdir)/missing \
	$(srcdir)/mkinstalldirs \
	$(srcdir)/gtk-doc.make \
	`find "$(srcdir)" -type f -name Makefile.in -print`
