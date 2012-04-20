if PLATFORM_WIN32
no_undefined = -no-undefined
endif

if OS_WIN32

install-libtool-import-lib:
	$(INSTALL) .libs/libots-1.dll.a $(DESTDIR)$(libdir)

uninstall-libtool-import-lib:
	-rm $(DESTDIR)$(libdir)/libots-1.dll.a
else
install-libtool-import-lib:
uninstall-libtool-import-lib:
endif

DEFS = @DEFS@ -DVERSION=\"@OTS_VERSION@\" -DDICTIONARY_DIR=\"$(pkgdatadir)/\"

INCLUDES = -I$(top_srcdir) $(OTS_CFLAGS) $(ANSI_CFLAGS)

lib_LTLIBRARIES = libots-1.la

libots_1_la_LIBADD  = $(OTS_LIBS)
libots_1_la_LDFLAGS = -version-info @VERSION_INFO@ $(no_undefined)
libots_1_la_SOURCES =		\
	libots.h		\
	article.c		\
	dictionary.c		\
	grader.c		\
	grader-tf.c		\
	grader-tc.c		\
	grader-tc.h		\
	html.c			\
	parser.c 		\
	text.c			\
	stemmer.c		\
	highlighter.c		\
	wordlist.c		\
	relations.c

libots_1_includedir = $(includedir)/libots-1/ots
libots_1_include_HEADERS = 	\
	libots.h

bin_PROGRAMS=ots
ldadd = $(top_builddir)/src/libots-1.la $(OTS_LIBS) $(POPT_LIBS)

ots_LDADD=$(ldadd)

install-data-local: install-libtool-import-lib

uninstall-local: uninstall-libtool-import-lib
