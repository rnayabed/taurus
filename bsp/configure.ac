AC_INIT([libtaurus], [1.0], [debayansutradhar3@gmail.com], [taurus], [https://github.com/rnayabed/taurus-sdk])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_RANLIB
AM_PROG_AR
AM_PROG_AS
AC_CONFIG_FILES([Makefile])
AC_ARG_ENABLE([debug],
[  --enable-debug    Turn on debugging],
[case "${enableval}" in
  yes) debug=true ;;
  no)  debug=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],[debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])
AC_OUTPUT

