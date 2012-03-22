dnl Process this file with autoconf to produce a configure script.

AC_INIT(librescribe, 0.1)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([1.11])
AM_SILENT_RULES([yes])
AC_PROG_CXX

dnl *************************************************************************
dnl Internationalization
dnl *************************************************************************
#IT_PROG_INTLTOOL([0.35.0])

#GETTEXT_PACAKGE=librescribe
#AC_SUBST(GETTEXT_PACKAGE)
#AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [GETTEXT package name])

#AM_GLIB_GNU_GETTEXT

#LT_INIT

dnl Check for required packages.

PKG_CHECK_MODULES([glib], [glib-2.0])
PKG_CHECK_MODULES([libxmlxx], [libxml++-2.6])
PKG_CHECK_MODULES([libusb], [libusb-1.0])
PKG_CHECK_MODULES([openobex], [openobex])
PKG_CHECK_MODULES([Python], [python-2.7])
PKG_CHECK_MODULES([udev], [udev])

dnl Check for wxWidgets
AM_OPTIONS_WXCONFIG
reqwx=2.8.0
AM_PATH_WXCONFIG($reqwx, wxWin=1)
if [ test "$wxWin" != 1 ]; then
    AC_MSG_ERROR([
wxWidgets must be installed on your system.

Please check that wx-config is in your path, the directory where wxWidgets
libraries are installed (returned by 'wx-config --libs' or
'wx-config --static --libs' command) is in LD_LIBRARY_PATH or equivalent
variable, and wxWidgets version is $(reqwx) or above])
fi

AC_OUTPUT([
Makefile
src/Makefile
])

