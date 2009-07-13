# This was pulled from google:
AC_DEFUN(PKG_CHECK_MODULES, [
  succeeded=no

  if test -z "$PKG_CONFIG"; then
    AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  fi

  if test "$PKG_CONFIG" = "no" ; then
     echo "*** The pkg-config script could not be found. Make sure it is"
     echo "*** in your path, or set the PKG_CONFIG environment variable"
     echo "*** to the full path to pkg-config."
     echo "*** Or see http://www.freedesktop.org/software/pkgconfig to get 
pkg-config."
  else
     PKG_CONFIG_MIN_VERSION=0.9.0
     if $PKG_CONFIG --atleast-pkgconfig-version $PKG_CONFIG_MIN_VERSION; then
        AC_MSG_CHECKING(for $2)

        if $PKG_CONFIG --exists "$2" ; then
            AC_MSG_RESULT(yes)
            succeeded=yes

            AC_MSG_CHECKING($1_CFLAGS)
            $1_CFLAGS=`$PKG_CONFIG --cflags "$2"`
            AC_MSG_RESULT($$1_CFLAGS)

            AC_MSG_CHECKING($1_LIBS)
            $1_LIBS=`$PKG_CONFIG --libs "$2"`
            AC_MSG_RESULT($$1_LIBS)
        else
            $1_CFLAGS=""
            $1_LIBS=""
            AC_MSG_RESULT([no])
        fi

        AC_SUBST($1_CFLAGS)
        AC_SUBST($1_LIBS)
     else
        echo "*** Your version of pkg-config is too old. You need version 
$PKG_CONFIG_MIN_VERSION or newer."
        echo "*** See http://www.freedesktop.org/software/pkgconfig";
     fi
  fi

  if test $succeeded = yes; then
     ifelse([$3], , :, [$3])
  else
     ifelse([$4], , AC_MSG_ERROR([Library requirements ($2) not met; consider 
adjusting the PKG_CONFIG_PATH environment variable if your libraries are in a 
nonstandard prefix so pkg-config can find them.]), [$4])
  fi
])

AC_INIT([XMonad Log Applet], [1.0.0], [awick@uhsure.com], [xmonad-log-applet])

AC_PREFIX_DEFAULT(/usr)
# standard compilers and tools
AC_PROG_CC
AC_PROG_SED
AC_PROG_INSTALL
AC_HEADER_STDC
# The only standard headers we use are string.h and stdlib.h, both
# of which are checked by AC_HEADER_STDC. So now it's only the gnome
# and dbus stuff.

PKG_CHECK_MODULES(GLIB, glib-2.0 >= 2.20.0)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

PKG_CHECK_MODULES(DBUS_GLIB, dbus-glib-1 >= 0.80)
AC_SUBST(DBUS_GLIB_CFLAGS)
AC_SUBST(DBUS_GLIB_LIBS)

PKG_CHECK_MODULES(LIBPANEL, libpanelapplet-2.0 >= 2.26.0)
AC_SUBST(LIBPANEL_CFLAGS)
AC_SUBST(LIBPANEL_LIBS)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
