## Process this file to produce Makefile.in
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src

librescribedocdir = ${prefix}/doc/librescribe
librescribedoc_DATA =\
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS

#INTLTOOL_FILES =\
#    intltool-extract.in\
#    intltool-merge.in\
#    intltool-update.in

#EXTRA_DIST =\
#    $(librescribedoc_DATA)\
#    $(INTLTOOL_FILES)

#DISTCLEANFILES =\
#    intltool-extract\
#    intltool-merge\
#    intltool-update\
#    po/.intltool-merge-cache

# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(librescribedocdir)
