## autoconf configure.ac && autoreconf -if
AC_INIT(DESCRIPTION)

AC_ARG_ENABLE(system-papi,
  AC_HELP_STRING([--enable-system-papi], [Use a PAPI installation already on the system; requires --papi-lib-path to be set.]),
  [BUILD_BUNDLED_PAPI="no"], [BUILD_BUNDLED_PAPI="yes"])

AC_ARG_WITH(papi-home,
  AC_HELP_STRING([--with-papi-home], [Path to PAPI installation.]),
  [PAPI_HOME=$withval])


if test "X${BUILD_BUNDLED_PAPI}" = "Xyes"; then
  PAPI_CFLAGS="-I./build/include/"
  PAPI_LIBS="./build/lib/libpapi.a ./build/lib/libpfm.a"
else
  PAPI_CFLAGS="-I${PAPI_HOME}/include"
  PAPI_LIBS="${PAPI_HOME}/libpapi.a"
fi


AC_PROG_CC([xlc icc gcc cc])

AC_SUBST(CPICFLAGS)
AC_SUBST(BUILD_BUNDLED_PAPI)
AC_SUBST(PAPI_CFLAGS)
AC_SUBST(PAPI_LIBS)


AC_OUTPUT(src/Makevars)
AC_OUTPUT(src/build_papi)
