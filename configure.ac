## autoconf configure.ac && autoreconf -if

################################################################################
# This file is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or distribute 
# this file, for any purpose, and by any means.
#
# In jurisdictions that recognize copyright laws, the author or authors of this 
# file dedicate any and all copyright interest in the file to the public
# domain. We make this dedication for the benefit of the public at large and to 
# the detriment of our heirs and successors. We intend this dedication to be an 
# overt act of relinquishment in perpetuity of all present and future rights to 
# this file under copyright law.
#
# THE FILE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
# AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN 
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
# WITH THIS FILE OR THE USE OR OTHER DEALINGS IN THIS FILE.
#
# For more information, please refer to http://unlicense.org/
################################################################################

AC_INIT(DESCRIPTION)
AC_PROG_CC([xlc icc gcc cc])

AC_ARG_ENABLE(ipcm,
  AC_HELP_STRING([--enable-ipcm], [Use the Intel Performance Counter Monitor instead of PAPI.]),
  [BUILD_IPCM="yes"], [BUILD_IPCM="no"])

AC_ARG_ENABLE(system-papi,
  AC_HELP_STRING([--enable-system-papi], [Use a PAPI installation already on the system; requires --papi-lib-path to be set.]),
  [BUILD_BUNDLED_PAPI="no"], [BUILD_BUNDLED_PAPI="yes"])

AC_ARG_WITH(papi-home,
  AC_HELP_STRING([--with-papi-home], [Path to PAPI installation.]),
  [PAPI_HOME=$withval])


if test "X${BUILD_IPCM}" = "Xyes"; then
  PAPI_CFLAGS="-I${PAPI_HOME}/include"
  PAPI_LIBS="build/libs/libcipcm.so build/libs/libpcm.so"
  OBEXTRA="instipcm"
else
  if test "X${BUILD_BUNDLED_PAPI}" = "Xyes"; then
    PAPI_CFLAGS="-I./build/include/"
    PAPI_LIBS="./build/lib/libpapi.a ./build/lib/libpfm.a"
    OBEXTRA="mkpapi"
  else
    PAPI_CFLAGS="-I${PAPI_HOME}/include"
    PAPI_LIBS="${PAPI_HOME}/lib/libpapi.a ${PAPI_HOME}/lib/libpfm.a"
    OBEXTRA=""
  fi
fi



### Report
echo " "
echo "****************** Results of pbdPAPI package configure *****************"
echo " "
echo ">> CC = ${CC}"
echo ">> Build IPCM: ${BUILD_IPCM}"
echo ">> Build Bundled PAPI: ${BUILD_BUNDLED_PAPI}"
echo ">> PAPI_HOME = ${PAPI_HOME}"
echo " "
echo "************************************************************************"
echo " "



AC_SUBST(BUILD_BUNDLED_PAPI)
AC_SUBST(PAPI_CFLAGS)
AC_SUBST(PAPI_LIBS)
AC_SUBST(OBEXTRA)
AC_OUTPUT(src/Makevars)

AC_SUBST(CC)
AC_OUTPUT(src/build_papi)

AC_SUBST(BUILD_IPCM)
AC_OUTPUT(R/whichlib.r)
