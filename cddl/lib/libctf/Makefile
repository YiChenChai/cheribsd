.PATH: ${SRCTOP}/cddl/contrib/opensolaris/common/ctf
.PATH: ${SRCTOP}/cddl/contrib/opensolaris/lib/libctf/common
.PATH: ${SRCTOP}/sys/cddl/contrib/opensolaris/common/ctf

LIB=		ctf
SRCS=		ctf_create.c \
		ctf_decl.c \
		ctf_error.c \
		ctf_hash.c \
		ctf_labels.c \
		ctf_lib.c \
		ctf_lookup.c \
		ctf_open.c \
		ctf_subr.c \
		ctf_types.c \
		ctf_util.c
MAN=		ctf.5

WARNS?=		2
CFLAGS+=	-DCTF_OLD_VERSIONS

CFLAGS+= -DIN_BASE
CFLAGS+= -I${ZFSTOP}/include
CFLAGS+= -I${ZFSTOP}/lib/libspl/include/
CFLAGS+= -I${ZFSTOP}/lib/libspl/include/os/freebsd
CFLAGS+= -include ${ZFSTOP}/include/os/freebsd/spl/sys/ccompile.h
CFLAGS+= -DHAVE_ISSETUGID

CFLAGS+=	-I${SRCTOP}/sys/cddl/compat/opensolaris \
		-I${SRCTOP}/cddl/compat/opensolaris/include \
		-I${OPENSOLARIS_USR_DISTDIR}/head \
		-I${OPENSOLARIS_USR_DISTDIR}/common/ctf \
		-I${OPENSOLARIS_USR_DISTDIR}/lib/libctf/common \
		-I${OPENSOLARIS_SYS_DISTDIR}/uts/common

LIBADD+=	spl z

.include <bsd.lib.mk>
