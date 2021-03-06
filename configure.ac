dnl
dnl JDim用 configure.ac
dnl
AC_PREREQ(2.50)
AC_INIT(jdim, 1.0)
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS(config.h)

AC_ISC_POSIX
AC_PROG_CC
AC_HEADER_STDC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_LIBTOOL
AC_PROG_MKDIR_P
AC_LANG_CPLUSPLUS

AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])

dnl
dnl buildinfo.h
dnl
AC_DEFINE(HAVE_BUILDINFO_H, 1, Define to 1 if you have the 'buildinfo.h' header file.)
AC_PATH_PROG(SVN, svn)
AC_PATH_PROG(SVNVERSION, svnversion)
AC_PATH_PROG(GIT, git)
AC_PATH_PROG(XSUM, md5sum, [cksum])
AC_SUBST(ac_configure_args)

dnl
dnl OSを判定してOS別の設定
dnl 
case "${host_os}" in 
 freebsd*) 
   AC_DEFINE(USE_MKTIME, , [use mktime])
   ;;
 solaris*)
   AC_DEFINE(NO_TIMEGM, , [no timegm])
   ;;
 mingw*)
   AC_DEFINE(USE_MKTIME, , [use mktime])
   ;;
 darwin*)
   AC_DEFUN([AM_ICONV], [])
   AC_DEFINE(USE_MKTIME, , [use mktime])
   ;;
dnl linux*|gnu*|*-gnu
 *)
   AC_DEFUN([AM_ICONV], [])
   ;;
esac

AM_ICONV


dnl ---------------------------------------------------
dnl ---------------------------------------------------
dnl
dnl ユーザー設定
dnl

dnl 追加コンパイルオプション
dnl -Wextraで有効になる-Wunused-parameterは修正方法の検討が必要なので暫定的に無効
CXXFLAGS="-ggdb -Wall -Wextra -Wno-unused-parameter -pedantic $CXXFLAGS"

dnl ---------------------------------------------------
dnl ---------------------------------------------------

LIBSM_CFLAGS=""
LIBSM_LIBS=""
GNOMEUI_CFLAGS=""
GNOMEUI_LIBS=""

dnl
dnl パッケージのチェック
dnl
PKG_CHECK_MODULES(GTHREAD, [gthread-2.0 >= 2.0] )

AC_SUBST(GTHREAD_CFLAGS)
AC_SUBST(GTHREAD_LIBS)


dnl
dnl checking gtkmm
dnl
AC_MSG_CHECKING(for --with-gtkmm3)
AC_ARG_WITH(gtkmm3,
  [AS_HELP_STRING([--with-gtkmm3], [use gtkmm-3.0 instead of gtkmm-2.4 [default=no]])],
  [with_gtkmm3="$withval"],
  [with_gtkmm3=no])
AC_MSG_RESULT($with_gtkmm3)

AS_IF([test "x$with_gtkmm3" = xno],
      [PKG_CHECK_MODULES(GTKMM, [gtkmm-2.4 >= 2.18.0], [], [
         AC_MSG_WARN([JDim will require gtkmm-2.4 >= 2.18.0 in the future release.])
         PKG_CHECK_MODULES(GTKMM, [gtkmm-2.4 >= 2.4.0])
       ])],
      [PKG_CHECK_MODULES(GTKMM, [gtkmm-3.0 >= 3.0.0], [], [])
       AC_DEFINE(WITH_GTKMM_3_0, 1, [Use gtkmm-3.0 instead of gtkmm-2.4])])

AC_SUBST(GTKMM_CFLAGS)
AC_SUBST(GTKMM_LIBS)


dnl
dnl crypt
dnl
AC_CHECK_HEADERS([crypt.h])
AC_CHECK_LIB(crypt,crypt)


dnl
dnl zlib
dnl
AC_CHECK_HEADERS([zlib.h])
AC_CHECK_LIB(z,inflate)


dnl
dnl packages dependent on platform 
dnl
use_windres=""

case "${host_os}" in 
 mingw*)
   dnl
   dnl winsock2 using model of socket
   dnl not available uname on windows
   dnl
   
   AC_CHECK_HEADERS([winsock2.h])
   AC_CHECK_LIB(ws2_32,_head_libws2_32_a)
   
   AC_CHECK_HEADERS([regex.h])
   AC_CHECK_LIB(regex,regexec)
   
   AC_CHECK_HEADERS([iconv.h])
   AC_CHECK_LIB(iconv,_head_libiconv_a)
   AC_CHECK_TOOL([WINDRES], [windres], [windres])
   AC_SUBST(WINDRES)
   AC_DEFINE(USE_WINDRES, , [use windres])
   use_windres="yes"
   ;;
 *)
   dnl
   dnl any other POSIX systems
   dnl

   AC_CHECK_HEADERS([sys/utsname.h])

   AC_CHECK_HEADERS([socket.h])
   AC_CHECK_LIB(socket,socket)
   ;;
esac

AM_CONDITIONAL(MAKE_WINDRES, test -n "$use_windres")


dnl
dnl X関連ライブラリ
dnl
X11_CFLAGS=""
X11_LIBS=""

case "${host_os}" in 
 mingw*)
   ;;
 *)
   PKG_CHECK_MODULES(X11, x11)
   AC_SUBST(X11_CFLAGS)
   AC_SUBST(X11_LIBS)
   ;;
esac


dnl
dnl セッション管理
dnl
AC_MSG_CHECKING(for --with-sessionlib)
AC_ARG_WITH(sessionlib,
AC_HELP_STRING([--with-sessionlib=@<:@xsmp|gnomeui|no@:>@],
               [use session control library @<:@default=xsmp@:>@]),
[with_sessionlib="$withval"], [with_sessionlib=xsmp])
AC_MSG_RESULT($with_sessionlib)

AS_IF(
  [test "x$with_sessionlib" = xno],
  [],

  dnl XSMPを使ってセッション管理をする。libSMとlibICEが必要。無ければXSMPは無効になる
  [test "x$with_sessionlib" = xxsmp],
  [PKG_CHECK_MODULES(LIBSM, [sm >= 1.2], [], [ac_sm_ice_found=no])
   PKG_CHECK_MODULES(LIBICE, [ice >= 1.0], [], [ac_sm_ice_found=no])
   AS_IF(
     [test "x$ac_sm_ice_found" = xno],
     [AC_MSG_NOTICE([Disable XSMP due to LIBSM or LIBICE not found])],

     [LIBSM_CFLAGS="$LIBSM_CFLAGS $LIBICE_CFLAGS"
      LIBSM_LIBS="$LIBSM_LIBS $LIBICE_LIBS"
      AC_DEFINE(USE_XSMP, , [use xsmp])
      AC_SUBST(LIBSM_CFLAGS)
      AC_SUBST(LIBSM_LIBS)]
   )],

  [test "x$with_sessionlib" = xgnomeui -a "x$with_gtkmm3" != xno],
  [AC_MSG_ERROR([gnomeui won't support for gtkmm3])],

  [test "x$with_sessionlib" = xgnomeui],
  [PKG_CHECK_MODULES(GNOMEUI, [libgnomeui-2.0 >= 2.0])
   AC_DEFINE(USE_GNOMEUI, , [use gnomeui])
   AC_SUBST(GNOMEUI_CFLAGS)
   AC_SUBST(GNOMEUI_LIBS)],

  [AC_MSG_ERROR([session control library not found])]
)


dnl
dnl checking oniguruma (deprecated option)
dnl
AC_ARG_WITH(oniguruma,
AS_HELP_STRING([--with-oniguruma], [DEPRECATED. use --with-regex=oniguruma]),
[AC_MSG_WARN([--with-oniguruma is deprecated. Use --with-regex=oniguruma instead.])
 with_regex=oniguruma])

dnl
dnl checking PCRE (deprecated option)
dnl
AC_ARG_WITH(pcre,
AS_HELP_STRING([--with-pcre], [DEPRECATED. use --with-regex=pcre]),
[AC_MSG_WARN([--with-pcre is deprecated. Use --with-regex=pcre instead.])
 with_regex=pcre])


dnl
dnl 正規表現ライブラリ
dnl
AC_MSG_CHECKING(for --with-regex)
AC_ARG_WITH(regex,
AC_HELP_STRING([--with-regex=@<:@posix|pcre|oniguruma@:>@],
               [use regular expression library @<:@default=posix@:>@]),
[], [with_regex=posix])
AC_MSG_RESULT($with_regex)

AS_IF(
  [test "x$with_regex" = xposix],
  [AC_CHECK_HEADERS([regex.h], , [AC_MSG_ERROR([regex.h not found])])
   AC_CHECK_LIB([regex], [regexec])],

  [test "x$with_regex" = xpcre],
  [PKG_CHECK_MODULES(PCRE, [libpcre >= 6.5])
   AC_DEFINE(USE_PCRE, , [use PCRE])
   AC_SUBST(PCRE_CFLAGS)
   AC_SUBST(PCRE_LIBS)
   AC_CHECK_HEADERS([pcreposix.h], , [AC_MSG_ERROR([pcreposix.h not found])])
   AC_CHECK_LIB([pcreposix], [regexec], , [AC_MSG_ERROR([libpcreposix.a not found])])],

  [test "x$with_regex" = xoniguruma],
  [PKG_CHECK_MODULES(ONIG, [oniguruma])
   AC_DEFINE(USE_ONIG, , [use oniguruma regular expressions library])
   AC_SUBST(ONIG_CFLAGS)
   AC_SUBST(ONIG_LIBS)
   AC_CHECK_HEADERS([onigposix.h], , [AC_MSG_ERROR([onigposix.h not found])])
   AC_CHECK_LIB([onig], [regexec], , [AC_MSG_ERROR([libonig.a not found])])],

  [AC_MSG_ERROR([regular expression library not found])]
)


dnl
dnl checking OpenSSL (deprecated option)
dnl
AC_ARG_WITH(openssl,
AS_HELP_STRING([--with-openssl], [DEPRECATED. use --with-tls=openssl]),
[AC_MSG_WARN([--with-openssl is deprecated. Use --with-tls=openssl instead.])
 with_tls=openssl])

dnl
dnl TLS
dnl
AC_MSG_CHECKING(for --with-tls)
AC_ARG_WITH(tls,
AC_HELP_STRING([--with-tls=@<:@gnutls|openssl@:>@],
               [use TLS library @<:@default=gnutls@:>@]),
[with_tls="$withval"], [with_tls=gnutls])
AC_MSG_RESULT($with_tls)

AS_IF(
  [test "x$with_tls" = xgnutls],
  [PKG_CHECK_MODULES(GNUTLS, [gnutls >= 1.2])
   AC_DEFINE(USE_GNUTLS, , [use gnutls])
   AC_CHECK_HEADERS([gcrypt.h])
   AC_CHECK_LIB(gcrypt, gcry_md_hash_buffer, [], [AC_MSG_ERROR([gcrypt not found])])
   AC_SUBST(GNUTLS_CFLAGS)
   AC_SUBST(GNUTLS_LIBS)],

  [test "x$with_tls" = xopenssl],
  [PKG_CHECK_MODULES(OPENSSL, [openssl >= 0.9])
   AC_DEFINE(USE_OPENSSL, , [use openssl])
   AC_SUBST(OPENSSL_CFLAGS)
   AC_SUBST(OPENSSL_LIBS)],

  [AC_MSG_ERROR([TLS library not found])]
)


dnl
dnl enable gprof
dnl
AC_MSG_CHECKING(for --enable-gprof)
AC_ARG_ENABLE(gprof,
AC_HELP_STRING([--enable-gprof],[enable gprof]),
[enable_gprof="$enableval"], [enable_gprof=no])
AC_MSG_RESULT($enable_gprof)

AS_IF(
  [test "x$enable_gprof" = xyes],
  [CXXFLAGS="$CXXFLAGS -pg"]
)


dnl
dnl checking migemo
dnl
AC_MSG_CHECKING(for --with-migemo)
AC_ARG_WITH(migemo,
AC_HELP_STRING([--with-migemo],[enable migemo search]),
[with_migemo="$withval"], [with_migemo=no])
AC_MSG_RESULT($with_migemo)

AS_IF(
  [test "x$with_migemo" = xyes],
  [AC_CHECK_HEADERS([migemo.h])
   AC_CHECK_LIB(migemo, migemo_open)]
)


AC_MSG_CHECKING(for --with-migemodict)
AC_ARG_WITH(migemodict,
AC_HELP_STRING([--with-migemodict=PATH],[specifiy the path of migemo dictionary]),
[with_migemodict="$withval"], [with_migemodict=])
AC_MSG_RESULT($with_migemodict)

AS_IF(
  [test -n "$with_migemodict"],
  [AC_DEFINE_UNQUOTED(MIGEMODICT, "$with_migemodict", [migemodict])]
)


dnl
dnl checking xdg-open
dnl
AC_MSG_CHECKING(for --with-xdgopen)
AC_ARG_WITH(xdgopen,
AC_HELP_STRING([--with-xdgopen],[use xdg-open as default browser]),
[with_xdgopen="$withval"], [with_xdgopen=no])
AC_MSG_RESULT($with_xdgopen)

AS_IF(
  [test "x$with_xdgopen" = xyes],
  [AC_DEFINE(XDGOPEN, , [use xdg-open])]
)


dnl
dnl checking alsa
dnl
case "${host_os}" in
  linux*|*linux)
  AC_MSG_CHECKING(for --with-alsa)
  AC_ARG_WITH(alsa,
  AC_HELP_STRING([--with-alsa],[enable alsa]),
  [with_alsa="$withval"], [with_alsa=no])
  AC_MSG_RESULT($with_alsa)

  AS_IF(
    [test "x$with_alsa" = xyes],
    [PKG_CHECK_MODULES(ALSA, [alsa >= 1.0])
     AC_DEFINE(USE_ALSA, , [use alsa])
     AC_SUBST(ALSA_CFLAGS)
     AC_SUBST(ALSA_LIBS)]
  )
 ;;
esac


dnl
dnl checking gthread (deprecated option)
dnl
AC_ARG_WITH(gthread,
AS_HELP_STRING([--with-gthread], [DEPRECATED. use --with-thread=glib]),
[AC_MSG_WARN([--with-gthread is deprecated. Use --with-thread=glib instead.])
 with_thread=glib])

dnl
dnl checking std::thread (deprecated option)
dnl
AC_ARG_WITH(stdthread,
AS_HELP_STRING([--with-stdthread], [DEPRECATED. use --with-thread=std]),
[AC_MSG_WARN([--with-stdthread is deprecated. Use --with-thread=std instead.])
 with_thread=std])

dnl
dnl thread
dnl
AC_MSG_CHECKING(for --with-thread)
AC_ARG_WITH(thread,
AC_HELP_STRING([--with-thread=@<:@posix|glib|std@:>@],
               [use thread library @<:@default=posix@:>@]),
[with_thread="$withval"], [with_thread=posix])
AC_MSG_RESULT($with_thread)

AS_IF(
  [test "x$with_thread" = xposix],
  [],

  [test "x$with_thread" = xglib],
  [AC_MSG_WARN([--with-thread=glib is deprecated. Use --with-thread=std instead.])
   AC_DEFINE(USE_GTHREAD, , [Use gthread instead of pthread])],

  [test "x$with_thread" = xstd],
  [AC_DEFINE(WITH_STD_THREAD, 1, [Use std::thread instead of pthread])],

  [AC_MSG_ERROR([thread library not found])]
)


dnl
dnl checking pangolayout
dnl
AC_MSG_CHECKING(for --with-pangolayout)
AC_ARG_WITH(pangolayout,
AC_HELP_STRING([--with-pangolayout],[use pangolayout]),
[with_pangolayout="$withval"], [with_pangolayout=no])
AC_MSG_RESULT($with_pangolayout)

AS_IF(
  [test "x$with_pangolayout" = xyes],
  [AC_DEFINE(USE_PANGOLAYOUT, , [use pangolayout])]
)


dnl
dnl CPU別の最適化オプション
dnl
if test "x$enable_gprof" = "xno"; then

    dnl
    dnl checking native (gcc >= 4.2 x86 & x86_64)
    dnl
	AC_ARG_WITH(native,[ --with-native    (use native)],
        [ if test "$withval" != "no"; then
            echo "use native"
            CXXFLAGS="$CXXFLAGS -march=native"
        fi ])

    dnl
    dnl checking core2duo
    dnl
    AC_ARG_WITH(core2duo,[ --with-core2duo    (use core2duo)],
        [ if test "$withval" != "no"; then
            echo "use core2duo"
            AC_MSG_WARN([--with-core2duo is deprecated. Use ./configure CXXFLAGS="-march=core2"])
            CXXFLAGS="$CXXFLAGS -march=pentium-m -msse3"
        fi ])

    dnl
    dnl checking athlon64
    dnl
    AC_ARG_WITH(athlon64,[ --with-athlon64    (use athlon64)],
        [ if test "$withval" != "no"; then
            echo "use athlon64"
            AC_MSG_WARN([--with-athlon64 is deprecated. Use ./configure CXXFLAGS="-march=athlon64"])
            CXXFLAGS="$CXXFLAGS -march=athlon64"
        fi ])

    dnl
    dnl checking atom
    dnl
    AC_ARG_WITH(atom,[ --with-atom    (use atom)],
        [ if test "$withval" != "no"; then
            echo "use atom"
            AC_MSG_WARN([--with-atom is deprecated. Use ./configure CXXFLAGS="-march=prescott"])
            CXXFLAGS="$CXXFLAGS -march=prescott"
        fi ])

    dnl
    dnl checking ppc7400
    dnl
    AC_ARG_WITH(ppc7400,[ --with-ppc7400    (use PowerPC7400)],
        [ if test "$withval" != "no"; then
            echo "use ppc7400"
            AC_MSG_WARN([--with-ppc7400 is deprecated. Use ./configure CXXFLAGS="-mcpu=7400 -maltivec -mabi=altivec"])
            CXXFLAGS="$CXXFLAGS -mcpu=7400 -maltivec -mabi=altivec"
        fi ])

    dnl
    dnl checking ppc7450
    dnl
    AC_ARG_WITH(ppc7450,[ --with-ppc7450    (use PowerPC7450)],
        [ if test "$withval" != "no"; then
            echo "use ppc7450"
            AC_MSG_WARN([--with-ppc7450 is deprecated. Use ./configure CXXFLAGS="-mcpu=7450 -maltivec -mabi=altivec"])
            CXXFLAGS="$CXXFLAGS -mcpu=7450 -maltivec -mabi=altivec"
        fi ])
fi


AC_ARG_VAR(GTEST_SRCDIR, [path overriding googletest framework source directory])


AC_OUTPUT(Makefile src/Makefile src/skeleton/Makefile src/jdlib/Makefile src/dbtree/Makefile src/dbimg/Makefile  src/bbslist/Makefile src/board/Makefile src/article/Makefile src/image/Makefile src/message/Makefile src/history/Makefile src/config/Makefile src/icons/Makefile src/sound/Makefile src/xml/Makefile src/control/Makefile test/Makefile)
