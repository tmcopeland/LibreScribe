dnl Process this file with autoconf to produce a configure script.

AC_INIT([librescribe],[0.1])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([1.11])
AM_SILENT_RULES([yes])
AC_PROG_CXX
AC_LANG([C++])

dnl *************************************************************************
dnl Internationalization
dnl *************************************************************************
#IT_PROG_INTLTOOL([0.35.0])

#GETTEXT_PACAKGE=librescribe
#AC_SUBST(GETTEXT_PACKAGE)
#AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [GETTEXT package name])

#ALL_LINGUAS="en"
#AM_GNU_GETTEXT_VERSION(0.18.1)
#AM_GLIB_GNU_GETTEXT

#LIBRESCRIBE_LOCALEDIR=[${datadir}/locale]
#AC_SUBST(LIBRESCRIBE_LOCALEDIR)

AC_HEADER_STDC
AC_HEADER_STDBOOL
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([select])
AC_CHECK_HEADERS([arpa/inet.h])
AC_CHECK_FUNCS([arpa/inet.h])

AC_C_INLINE
AC_FUNC_MALLOC
AC_PROG_RANLIB
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

LT_INIT

dnl Check for required packages.

PKG_CHECK_MODULES([glib], [glib-2.0])
PKG_CHECK_MODULES([libxmlxx], [libxml++-2.6])
PKG_CHECK_MODULES([libusb], [libusb-1.0])
PKG_CHECK_MODULES([openobex], [openobex])
PKG_CHECK_MODULES([python], [python-2.7])
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

AC_CONFIG_FILES([
Makefile
src/Makefile
])
AC_OUTPUT

