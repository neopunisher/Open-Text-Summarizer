# man_MANS = ots.1

DOC_MODULE=ots
DOC_MAIN_SGML_FILE=$(DOC_MODULE)-docs.sgml

# The directory containing the source code. Relative to $(srcdir)
DOC_SOURCE_DIR=..

# Extra options to pass to gtkdoc-scangobj
SCANGOBJ_OPTIONS=--type-init-func="g_type_init ()" 

# Extra options to supply to gtkdoc-scan
SCAN_OPTIONS=

# Extra options to supply to gtkdoc-mkdb
MKDB_OPTIONS=--sgml-mode

# Extra options to supply to gtkdoc-fixref
FIXXREF_OPTIONS=

# Used for dependencies
HFILE_GLOB=$(top_srcdir)/src/*.h
CFILE_GLOB=$(top_srcdir)/src/*.c

# Header files to ignore when scanning
IGNORE_HFILES=

# Images to copy into HTML directory
HTML_IMAGES =

# Extra SGML files that are included by $(DOC_MAIN_SGML_FILE)
content_files =

# Other files to distribute
extra_files =

# CFLAGS and LDFLAGS for compiling scan program. Only needed
# if $(DOC_MODULE).types is non-empty.
GTKDOC_CFLAGS = -I$(top_srcdir)

GTKDOC_LIBS =


include $(top_srcdir)/gtk-doc.make
