AC_INIT([eip_server], [0.2], [localhost])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([-Wall])
AM_SILENT_RULES
AM_SILENT_RULES([yes])
AM_PROG_AR
LT_INIT
AC_PROG_CC
AM_PROG_CC_C_O
AC_CONFIG_HEADERS([config/config.h])
AC_CONFIG_FILES([
    Makefile
    src/Makefile
])
AC_CONFIG_MACRO_DIR([m4])

PKG_CHECK_MODULES([MODBUS], [libmodbus])
AC_ARG_VAR([CIP_LIBS], [linker flags for OpENer])
AC_ARG_VAR([CIP_CFLAGS], [C compiler flags for OpENer])
CIP_CFLAGS="-I\$(top_srcdir)/../OpENer/source/src \
	-I\$(top_srcdir)/../OpENer/source/src/cip \
	-I\$(top_srcdir)/../OpENer/source/src/ports/POSIX/sample_application"
CIP_LIBS="-L\$(top_srcdir)/../OpENer/bin/posix/src/cip -lCIP\
	-L\$(top_srcdir)/../OpENer/bin/posix/src/enet_encap -lENET_ENCAP\
	-L\$(top_srcdir)/../OpENer/bin/posix/src/ports/POSIX \
		-lPOSIXPLATFORM"


AC_CHECK_FUNC(pthread_create, [],
    AC_CHECK_LIB(pthread, pthread_create, [],
	AC_MSG_ERROR([*** eip_server requires POSIX threads support])))

create_shared_lib=""
case "$host_os" in
    cygwin*)
	create_shared_lib="-no-undefined"
	;;
    *) 
esac

AC_SUBST(create_shared_lib)

AC_ARG_WITH(pkgconfdir,
    AS_HELP_STRING([--with-pkgconfdir=dir],
        [path where pkgconfig files are stored]),
    pkgconfdir="$withval", pkgconfdir="")
if test -z "$pkgconfdir"; then
    pkgconfdir="$libdir/pkgconfig"
fi
AC_DEFINE_UNQUOTED(PKGCONF_DIR, "$pkgconfdir",
		[directory containing pkgconfig files])
AC_SUBST(PKGCONF_DIR, "$pkgconfdir")

AC_OUTPUT
