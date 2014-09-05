dnl $Id$
dnl config.m4 for extension yidb

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(yidb, for yidb support,
Make sure that the comment is aligned:
[  --with-yidb             Include yidb support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(yidb, whether to enable yidb support,
dnl Make sure that the comment is aligned:
dnl [  --enable-yidb           Enable yidb support])

if test "$PHP_YIDB" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-yidb -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/yidb.h"  # you most likely want to change this
  dnl if test -r $PHP_YIDB/$SEARCH_FOR; then # path given as parameter
  dnl   YIDB_DIR=$PHP_YIDB
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for yidb files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       YIDB_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$YIDB_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the yidb distribution])
  dnl fi

  dnl # --with-yidb -> add include path
  dnl PHP_ADD_INCLUDE($YIDB_DIR/include)

  dnl # --with-yidb -> check for lib and symbol presence
  dnl LIBNAME=yidb # you may want to change this
  dnl LIBSYMBOL=yidb # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $YIDB_DIR/lib, YIDB_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_YIDBLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong yidb lib version or lib not found])
  dnl ],[
  dnl   -L$YIDB_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(YIDB_SHARED_LIBADD)

  PHP_NEW_EXTENSION(yidb, yidb.c, $ext_shared)
fi
