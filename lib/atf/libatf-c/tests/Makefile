.include <bsd.init.mk>

PACKAGE=	tests

TESTS_SUBDIRS=	detail

ATF=		${SRCTOP}/contrib/atf
.PATH:		${ATF}/atf-c
.PATH:		${ATF}/atf-c/detail

CFLAGS+=	-DATF_INCLUDEDIR='"${INCLUDEDIR}"'
CFLAGS+=	-I${ATF}

${PACKAGE}FILES+=	macros_h_test.c
${PACKAGE}FILES+=	unused_test.c

.for _T in	build_test \
		error_test \
		tc_test \
		tp_test \
		utils_test
ATF_TESTS_C+=	${_T}
SRCS.${_T}=	${_T}.c test_helpers.c
.endfor

.for _T in	atf_c_test \
		build_test \
		check_test \
		macros_test

.if ${CC} == "cc"
# These tests are broken when using a cross toolchain (they embed ${CC}
# and try to compile with it).
ATF_TESTS_C:=	${ATF_TESTS_C:N${_T}}
.endif
TEST_METADATA.${_T}+=	required_programs="cc"
.endfor

.include "Makefile.inc"
.include <bsd.test.mk>
