.include <src.opts.mk>

LIB=	proc

SRCS=	proc_bkpt.c		\
	proc_create.c		\
	proc_regs.c		\
	proc_sym.c		\
	proc_rtld.c		\
	proc_util.c

INCS=	libproc.h

CFLAGS+=	-I${.CURDIR}

LIBADD+=	cxxrt elf procstat rtld_db util z

.if ${MK_CDDL} != "no"
LIBADD+=	ctf
IGNORE_PRAGMA=	YES
CFLAGS+= -DIN_BASE
CFLAGS+= -I${ZFSTOP}/include
CFLAGS+= -I${ZFSTOP}/lib/libspl/include/
CFLAGS+= -I${ZFSTOP}/lib/libspl/include/os/freebsd
CFLAGS+= -include ${ZFSTOP}/include/os/freebsd/spl/sys/ccompile.h
CFLAGS+= -DHAVE_ISSETUGID -DHAVE_BOOLEAN -DHAVE_STRLCAT -DHAVE_STRLCPY
CFLAGS+=	-I${SRCTOP}/cddl/contrib/opensolaris/lib/libctf/common \
		-I${SRCTOP}/sys/cddl/contrib/opensolaris/uts/common \
		-I${SRCTOP}/sys/cddl/compat/opensolaris
.else
CFLAGS+=	-DNO_CTF
.endif

SHLIB_MAJOR=	5

MAN=

HAS_TESTS=
SUBDIR.${MK_TESTS}+= tests

.include <bsd.lib.mk>
