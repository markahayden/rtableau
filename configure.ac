## Process this file with autoconf to produce a configure script.
##
## Configure.ac for rtableau
##
## Copyright (C) 2016 Mark Hayden
##
## Licensed under GNU GPL 3 or later

# The version set here will propagate to other files from here
AC_INIT([rtableau], version-0.1)

# Checks for common programs using default macros
AC_PROG_CC

case "$OSTYPE" in
  darwin* )
        TAB_CFLAGS="-g -F/Library/Frameworks"
        TAB_LIBS="-framework TableauCommon -framework TableauExtract -framework TableauServer"
        ;;
  linux*)
        AC_PATH_PROG([TDE_CONFIG], [tdeserver64])
        if test "${TDE_CONFIG}" != ""; then
             TAB_CFLAGS=`${TAB_CONFIG} --cflags`
             TAB_LIBS=`${TAB_CONFIG} --libs`
        else
             AC_MSG_ERROR([tdeserver64 not found, did you install the Tableau SDK?])
        fi
        ;;
    *)
        #Default Case
        AC_MSG_ERROR([Your platform is not currently supported])
        ;;
esac

AC_SUBST(TAB_CFLAGS)
AC_SUBST(TAB_LIBS)

AC_OUTPUT(src/Makevars)
