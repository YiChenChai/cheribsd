JEMALLOCSRCS:= jemalloc.c arena.c background_thread.c base.c bin.c bitmap.c \
	ckh.c ctl.c div.c extent.c extent_dss.c extent_mmap.c hash.c hook.c \
	large.c log.c malloc_io.c mutex.c mutex_pool.c nstime.c pages.c \
	prng.c prof.c rtree.c safety_check.c sc.c stats.c sz.c tcache.c \
	test_hooks.c ticker.c tsd.c witness.c

CFLAGS+=-I${SRCTOP}/contrib/jemalloc/include
.if ${MK_JEMALLOC_LG_VADDR_WIDE} != no
CFLAGS+=-D_USE_LG_VADDR_WIDE
.endif

.if ${MK_MALLOC_REVOCATION_SHIM} == yes
MRS_REAL_PREFIX=__
.endif

.for src in ${JEMALLOCSRCS}
MISRCS+=jemalloc_${src}
CLEANFILES+=jemalloc_${src}
jemalloc_${src}: ${SRCTOP}/contrib/jemalloc/src/${src} .NOMETA
	ln -sf ${.ALLSRC} ${.TARGET}
.if ${MACHINE_ABI:Mpurecap}
CFLAGS.jemalloc_${src}+=	-Werror=cheri-bitwise-operations -Werror=cheri
.endif
.if ${MK_MALLOC_REVOCATION_SHIM} == yes
CFLAGS.jemalloc_${src}+=	-DMALLOC_REVOCATION_SHIM
.endif
.endfor

COMPARTMENT_POLICY+=${LIBC_SRCTOP}/stdlib/malloc/jemalloc/Compartments.json

MAN+=jemalloc.3
CLEANFILES+=jemalloc.3
jemalloc.3: ${SRCTOP}/contrib/jemalloc/doc/jemalloc.3 .NOMETA
	ln -sf ${.ALLSRC} ${.TARGET}

.if ${MK_MALLOC_REVOCATION_SHIM} == no || !${MACHINE_ABI:Mpurecap}
MLINKS+= \
	jemalloc.3 malloc.3 \
	jemalloc.3 calloc.3 \
	jemalloc.3 posix_memalign.3 \
	jemalloc.3 aligned_alloc.3 \
	jemalloc.3 realloc.3 \
	jemalloc.3 free.3
.endif

MLINKS+= \
	jemalloc.3 malloc_usable_size.3 \
	jemalloc.3 malloc_stats_print.3 \
	jemalloc.3 mallctl.3 \
	jemalloc.3 mallctlnametomib.3 \
	jemalloc.3 mallctlbymib.3 \
	jemalloc.3 mallocx.3 \
	jemalloc.3 rallocx.3 \
	jemalloc.3 xallocx.3 \
	jemalloc.3 sallocx.3 \
	jemalloc.3 dallocx.3 \
	jemalloc.3 sdallocx.3 \
	jemalloc.3 nallocx.3 \
	jemalloc.3 malloc.conf.5

.if ${MK_MALLOC_PRODUCTION} != "no" || defined(MALLOC_PRODUCTION)
CFLAGS+=	-DMALLOC_PRODUCTION
.endif
