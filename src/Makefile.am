## Process this file to produce Makefile.in
AM_CPPFLAGS =\
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\"\
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\"\
	-DPACAKGE_DATA_DIR=\""$(pkgdatadir)"\"\
	$(glib_CFLAGS)\
	$(libxmlxx_CFLAGS)\
	$(openobex_CFLAGS)\
	$(udev_CFLAGS)\
	$(WX_CPPFLAGS)

AM_CXXFLAGS =\
	$(WX_CXXFLAGS_ONLY)

AM_CFLAGS =\
	-Wall\
	-g\
	$(WX_CFLAGS_ONLY)

bin_PROGRAMS = librescribe

librescribe_SOURCES =\
	AboutDialog.h\
	AboutDialog.cpp\
	BackgroundMonitor.cpp\
	DeviceInfo.h\
	DeviceInfo.cpp\
	GUIFrame.h\
	GUIFrame.cpp\
	LibreScribe.h\
	LibreScribe.cpp\
	Smartpen.h\
	Smartpen.cpp\
	wx_pch.h

librescribe_LDFLAGS =\
	-Wl,--export-dynamic

librescribe_LDADD =\
	$(glib_LIBS)\
	$(libxmlxx_LIBS)\
	$(openobex_LIBS)\
	$(udev_LIBS)\
	$(WX_LIBS)

