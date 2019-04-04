dnl $Id$
dnl config.m4 for extension xannotation

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(xannotation, for xannotation support,
Make sure that the comment is aligned:
[  --with-xannotation             Include xannotation support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(xannotation, whether to enable xannotation support,
dnl Make sure that the comment is aligned:
dnl [  --enable-xannotation           Enable xannotation support])

if test "$PHP_XANNOTATION" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-xannotation -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/xannotation.h"  # you most likely want to change this
  dnl if test -r $PHP_XANNOTATION/$SEARCH_FOR; then # path given as parameter
  dnl   XANNOTATION_DIR=$PHP_XANNOTATION
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for xannotation files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       XANNOTATION_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$XANNOTATION_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the xannotation distribution])
  dnl fi

  dnl # --with-xannotation -> add include path
  dnl PHP_ADD_INCLUDE($XANNOTATION_DIR/include)

  dnl # --with-xannotation -> check for lib and symbol presence
  dnl LIBNAME=xannotation # you may want to change this
  dnl LIBSYMBOL=xannotation # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $XANNOTATION_DIR/$PHP_LIBDIR, XANNOTATION_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_XANNOTATIONLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong xannotation lib version or lib not found])
  dnl ],[
  dnl   -L$XANNOTATION_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(XANNOTATION_SHARED_LIBADD)

  PHP_NEW_EXTENSION(
    xannotation, 
    kernel/entries.c              \
    kernel/app/di.c              \
    kernel/app/session.c          \
    kernel/app/net.c              \
    kernel/db/adapter.c           \
    kernel/db/model.c             \
    kernel/app/request.c          \
    kernel/app/response.c         \
    xannotation.c, 
    $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
