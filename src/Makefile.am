## Process this file to produce Makefile.in
WORKDIR = `pwd`
PROJECT_DIRECTORY = $(WORKDIR)/..

OBJDIR_DEBUG = $(PROJECT_DIRECTORY)/obj/Debug
OUT_DEBUG = $(WORKDIR)/../bin/Debug/LibreScribe
DEP_DEBUG = 

OBJDIR_RELEASE = $(PROJECT_DIRECTORY)/obj/Release
OUT_RELEASE = $(PROJECT_DIRECTORY)/bin/Release/LibreScribe
DEP_RELEASE = 

OBJ_DEBUG =\
	$(OBJDIR_DEBUG)/AboutDialog.o\
	$(OBJDIR_DEBUG)/BackgroundMonitor.o\
	$(OBJDIR_DEBUG)/DeviceInfo.o\
	$(OBJDIR_DEBUG)/GUIFrame.o\
	$(OBJDIR_DEBUG)/LibreScribe.o\
	$(OBJDIR_DEBUG)/Smartpen.o

OBJ_RELEASE =\
	$(OBJDIR_RELEASE)/AboutDialog.o\
	$(OBJDIR_RELEASE)/BackgroundMonitor.o\
	$(OBJDIR_RELEASE)/DeviceInfo.o\
	$(OBJDIR_RELEASE)/GUIFrame.o\
	$(OBJDIR_RELEASE)/LibreScribe.o\
	$(OBJDIR_RELEASE)/Smartpen.o

AM_CPPFLAGS =\
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\"\
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\"\
	-DPACAKGE_DATA_DIR=\""$(pkgdatadir)"\"\
	-DWX_PRECOMP\
	$(libxmlxx_CFLAGS)\
	$(WX_CPPFLAGS)

AM_CXXFLAGS =\
	$(WX_CXXFLAGS_ONLY)

AM_CFLAGS =\
	-Wall\
	-O3\
	-g\
	$(glib_CFLAGS)\
	$(libusb_CFLAGS)\
	$(openobex_CFLAGS)\
	$(Python_CFLAGS)\
	$(udev_CFLAGS)\
	$(WX_CFLAGS_ONLY)

bin_PROGRAMS = librescribe

librescribe_SOURCES =\
	AboutDialog.h\
	AboutDialog.cc\
	BackgroundMonitor.cc\
	DeviceInfo.h\
	DeviceInfo.cc\
	GUIFrame.h\
	GUIFrame.cc\
	LibreScribe.h\
	LibreScribe.cc\
	Smartpen.h\
	Smartpen.cc\
	wx_pch.h

librescribe_LDFLAGS =\
	-Wl,--export-dynamic\
	-Winvalid-pch\
	-include wx_pch.h

librescribe_LDADD =\
	$(glib_LIBS)\
	$(libusb_LIBS)\
	$(libxmlxx_LIBS)\
	$(openobex_LIBS)\
	$(Python_LIBS)\
	$(udev_LIBS)\
	$(WX_LIBS)

librescribe_LINK = `sh -c -x "g++"`

#all: before-build build-debug build-release after-build
all: info before-build build-debug after-build

#clean: clean-debug clean-release
clean-local: clean-debug

info-local:
	echo -e "Python cflags: $(Python_CFLAGS)"
	echo -e "Python libs: $(Python_LIBS)"

before-build:
	cd $(PROJECT_DIRECTORY); rm -rfv ./bin
	false && cd $(PROJECT_DIRECTORY)/libusb/; [ -f configure ] || sh autogen.sh
	false && cd $(PROJECT_DIRECTORY)/libusb/; [ -f Makefile ] || ./configure
	false && cd $(PROJECT_DIRECTORY)/libusb/; [ -f libusb/.libs/libusb-1.0.a ] || make

after-build:
	cd $(PROJECT_DIRECTORY)
	find ./bin/ -mindepth 1 -maxdepth 1 -type d | xargs -n 1 cp -rfv -L ./res ./xdg
	find ./bin/ -mindepth 1 -maxdepth 1 -type d | xargs -n 1 cp -fv -L ./stf.py ./convert_stfs.sh

before-debug:
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after-debug:
	
build-debug:
	before-debug out-debug after-debug

debug: before-build build-debug after-build

out-debug: $(OBJ_DEBUG) $(DEP_DEBUG)
	$(librescribe_LINK) $(librescribe_LDFLAGS) $(librescribe_LDADD) $(OBJ_DEBUG) -o $(OUT_DEBUG)

$(OBJDIR_DEBUG)/AboutDialog.o: AboutDialog.cc
	$(CXX) $(AM_CPPFLAGS) $(AM_CXXFLAGS) $(AM_CFLAGS) $(librescribe_LDADD) $(librescribe_LDFLAGS) -c AboutDialog.cc -o $(OBJDIR_DEBUG)/AboutDialog.o

$(OBJDIR_DEBUG)/BackgroundMonitor.o: BackgroundMonitor.cc
	$(CXX) $(AM_CPPFLAGS) $(AM_CXXFLAGS) $(AM_CFLAGS) $(librescribe_LDADD) $(librescribe_LDFLAGS) -c BackgroundMonitor.cc -o $(OBJDIR_DEBUG)/BackgroundMonitor.o

$(OBJDIR_DEBUG)/DeviceInfo.o: DeviceInfo.cc
	$(CXX) $(AM_CPPFLAGS) $(AM_CXXFLAGS) $(AM_CFLAGS) $(librescribe_LDADD) $(librescribe_LDFLAGS) -c DeviceInfo.cc -o $(OBJDIR_DEBUG)/DeviceInfo.o

$(OBJDIR_DEBUG)/GUIFrame.o: GUIFrame.cc
	$(CXX) $(AM_CPPFLAGS) $(AM_CXXFLAGS) $(AM_CFLAGS) $(librescribe_LDADD) $(librescribe_LDFLAGS) -c GUIFrame.cc -o $(OBJDIR_DEBUG)/GUIFrame.o

$(OBJDIR_DEBUG)/LibreScribe.o: LibreScribe.cc
	$(CXX) $(AM_CPPFLAGS) $(AM_CXXFLAGS) $(AM_CFLAGS) $(librescribe_LDADD) $(librescribe_LDFLAGS) -c LibreScribe.cc -o $(OBJDIR_DEBUG)/LibreScribe.o

$(OBJDIR_DEBUG)/Smartpen.o: Smartpen.cc
	$(CXX) $(AM_CPPFLAGS) $(AM_CXXFLAGS) $(AM_CFLAGS) $(librescribe_LDADD) $(librescribe_LDFLAGS) -c Smartpen.cc -o $(OBJDIR_DEBUG)/Smartpen.o

clean-debug:
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf $(PROJECT_DIRECTORY)/bin/Debug
	rm -rf $(OBJDIR_DEBUG)

