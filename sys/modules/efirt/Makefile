.PATH: ${SRCTOP}/sys/${MACHINE}/${MACHINE}
.PATH: ${SRCTOP}/sys/dev/efidev

KMOD=	efirt
SRCS=	efirt.c efirt_machdep.c efidev.c
SRCS+=	efirtc.c
SRCS+=  device_if.h bus_if.h clock_if.h

.if ${MACHINE_CPUARCH} == "amd64"
SRCS+=	opt_hwpmc_hooks.h opt_kstack_pages.h
.endif

.if ${MACHINE_CPUARCH} == "aarch64" || ${MACHINE_CPUARCH} == "amd64"
SRCS+=	efirt_support.S
DPSRCS+= assym.inc
efirt_support.o:	efirt_support.S assym.inc
	${CC} -c -x assembler-with-cpp -DLOCORE ${CFLAGS} \
	    ${.IMPSRC} -o ${.TARGET}
.endif

EXPORT_SYMS=	YES

.include <bsd.kmod.mk>
