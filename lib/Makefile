.include <src.opts.mk>

# The SUBDIR_BOOTSTRAP list is a small set of libraries which are used by many
# of the other libraries.  These are built first with a .WAIT between them
# and the main list to avoid needing a SUBDIR_DEPEND line on every library
# naming just these few items.

SUBDIR_BOOTSTRAP= \
	csu \
	.WAIT \
	libnetbsd \
	libc \
	libc_nonshared \
	libcompiler_rt \
	${_libcheribsdtest_dynamic} \
	${_libclang_rt} \
	cheri-libc++ \
	cheri-libc++experimental \
	libcxxrt \
	libdiff \
	libelf \
	libssp \
	libssp_nonshared \
	libsys \
	msun

# The main list; please keep these sorted alphabetically.
# The only exception is sqlite3: we place it at the start of the list since it
# takes a long time to build and starting it first improves parallelism.

SUBDIR=	${SUBDIR_BOOTSTRAP} \
	.WAIT \
	libsqlite3 \
	geom \
	lib9p \
	libalias \
	libarchive \
	libauditd \
	libbegemot \
	libblocksruntime \
	libbsddialog \
	libbsdstat \
	libbsm \
	libbz2 \
	libcalendar \
	libcam \
	libcapsicum \
	libcasper \
	libcompat \
	libcrypt \
	libdevctl \
	libdevdctl \
	libdevinfo \
	libdevstat \
	libdl \
	libdwarf \
	libedit \
	libelftc \
	libevent1 \
	libexecinfo \
	libexpat \
	libfetch \
	libgcc_eh \
	libgcc_s \
	libgeom \
	libifconfig \
	libipsec \
	libiscsiutil \
	libjail \
	libkiconv \
	libkldelf \
	libkvm \
	liblua \
	liblzma \
	libmemstat \
	${_libstatcounters} \
	libmd \
	libmixer \
	libmt \
	lib80211 \
	${_libnetmap} \
	libnv \
	libnvmf \
	libopenbsd \
	libpam \
	libpathconv \
	libpcap \
	libpjdlog \
	libproc \
	libprocstat \
	libregex \
	librpcsvc \
	librss \
	librt \
	librtld_db \
	libsbuf \
	libsmb \
	libstdbuf \
	libstdthreads \
	libsysdecode \
	libtacplus \
	libthr \
	libucl \
	libufs \
	libugidfw \
	libulog \
	libutil \
	${_libvgl} \
	libwrap \
	libxo \
	liby \
	libz \
	libzstd \
	ncurses \
	nss_tacplus

.if ${MACHINE_ABI:Mpurecap} && ${MACHINE_CPUARCH} == "aarch64"
.if !defined(COMPAT_LIBCOMPAT) || ${COMPAT_LIBCOMPAT} == "64CB"
SUBDIR+=	c18n
.endif
.endif

.if !${MACHINE_ABI:Mpurecap}
# Needs changes to be useful for purecap and not used by anything (yet)
SUBDIR+=	libthread_db
# lots of non-CHERI-friendly data structurs
SUBDIR+=	libnetmap
.endif

# Inter-library dependencies.  When the makefile for a library contains LDADD
# libraries, those libraries should be listed as build order dependencies here.

SUBDIR_DEPEND_geom=	libufs
SUBDIR_DEPEND_googletest= libregex
SUBDIR_DEPEND_libarchive= libz libbz2 libexpat liblzma libmd libzstd
SUBDIR_DEPEND_libauditdm= libbsm
SUBDIR_DEPEND_libbsddialog= ncurses
SUBDIR_DEPEND_libbsnmp= ${_libnetgraph}
SUBDIR_DEPEND_cheri-libc++:= libcxxrt
# libssp_nonshared doesn't need to be linked into libc on every arch, but it is
# small enough to build that this bit of serialization is likely insignificant.
SUBDIR_DEPEND_libc= libsys libcompiler_rt libssp_nonshared
SUBDIR_DEPEND_libcam= libsbuf
SUBDIR_DEPEND_libcasper= libnv
SUBDIR_DEPEND_libcrypt= libmd
SUBDIR_DEPEND_libdevstat= libkvm
SUBDIR_DEPEND_libdpv= libfigpar ncurses libutil
SUBDIR_DEPEND_libedit= ncurses
SUBDIR_DEPEND_libgeom= libexpat libsbuf
SUBDIR_DEPEND_librpcsec_gss= libgssapi
SUBDIR_DEPEND_libmagic= libz
SUBDIR_DEPEND_libmemstat= libkvm
SUBDIR_DEPEND_libpam= libcrypt ${_libradius} librpcsvc libtacplus libutil ${_libypclnt} ${_libcom_err}
SUBDIR_DEPEND_libpjdlog= libutil
SUBDIR_DEPEND_libprocstat= libkvm libutil
SUBDIR_DEPEND_libradius= libmd
SUBDIR_DEPEND_libsmb= libkiconv
# See comment above about libssp_nonshared
SUBDIR_DEPEND_libsys= libcompiler_rt libssp_nonshared
SUBDIR_DEPEND_libtacplus= libmd
SUBDIR_DEPEND_libulog= libmd
SUBDIR_DEPEND_libunbound= ${_libldns}
SUBDIR_DEPEND_liblzma= libthr
.if ${MK_OFED} != "no"
SUBDIR_DEPEND_libpcap= ofed
.endif
SUBDIR_DEPEND_nss_tacplus= libtacplus

# NB: keep these sorted by MK_* knobs

SUBDIR.${MK_BEARSSL}+=	libbearssl libsecureboot
SUBDIR.${MK_BLACKLIST}+=libblacklist
SUBDIR.${MK_BLUETOOTH}+=libbluetooth libsdp
SUBDIR.${MK_BSNMP}+=	libbsnmp

.if ${MACHINE_ABI:Mpurecap} && ${MK_CHERI_CAPREVOKE} == "yes"
SUBDIR+=	libcheri_caprevoke
SUBDIR_DEPEND_libc+=	libcheri_caprevoke
.endif

.if ${MACHINE_CPU:Mcheri}
_libcheribsdtest_dynamic=	libcheribsdtest_dynamic
.endif

.if ${MACHINE_CPUARCH} == "aarch64" || ${MACHINE_CPUARCH} == "riscv"
.if !${MACHINE_ABI:Mptr32}
SUBDIR+= libsimple_printf
.endif
.endif

.if !defined(COMPAT_LIBCOMPAT)
.if ${MK_CLANG} != "no" || ${MK_LLD} != "no" || \
    ${MK_LLDB} != "no" || ${MK_LLVM_BINUTILS} != "no"
SUBDIR+=	clang
.endif
.endif

SUBDIR.${MK_CUSE}+= 	libcuse
SUBDIR.${MK_TOOLCHAIN}+=libpe
SUBDIR.${MK_DIALOG}+=	libdpv libfigpar
SUBDIR.${MK_FDT}+=	libfdt
SUBDIR.${MK_FILE}+=	libmagic
SUBDIR.${MK_GPIO}+=	libgpio
SUBDIR.${MK_GSSAPI}+=	libgssapi librpcsec_gss
SUBDIR.${MK_ICONV}+=	libiconv_modules
SUBDIR.${MK_KERBEROS_SUPPORT}+=	libcom_err
SUBDIR.${MK_LDNS}+=	libldns
SUBDIR.${MK_STATS}+=	libstats

# The libraries under libclang_rt can only be built by clang.
.if ${COMPILER_TYPE} == "clang" && ${MK_CLANG} != "no" && \
    !${MACHINE_ABI:Mpurecap}
_libclang_rt=	libclang_rt
.elif (${MK_ASAN} != "no" || ${MK_UBSAN} != "no") && make(all)
.error Requested build with sanitizers but cannot build runtime libraries!
.endif

SUBDIR.${MK_EFI}+=	libefivar
SUBDIR.${MK_GOOGLETEST}+=	googletest
SUBDIR.${MK_NETGRAPH}+=	libnetgraph
SUBDIR.${MK_NIS}+=	libypclnt

.if ${MACHINE_CPUARCH} == "i386" || ${MACHINE_CPUARCH} == "amd64"
_libvgl=	libvgl
.endif

.if ${MACHINE_CPUARCH} == "aarch64"
SUBDIR.${MK_PMC}+=	libopencsd
.endif

.if ${MACHINE_CPUARCH} == "amd64"
SUBDIR.${MK_PMC}+=	libipt
.endif

.if (${MACHINE_CPUARCH} == "amd64" || ${MACHINE_CPUARCH} == "aarch64") && \
    !defined(COMPAT_LIBCOMPAT)
SUBDIR.${MK_BHYVE}+=	libvmmapi
.endif

.if ${MACHINE_ARCH:Mriscv*}
_libstatcounters=	libstatcounters
.endif

.if ${MACHINE_ARCH} != "powerpc" && ${MACHINE_CPUARCH} != "arm"
SUBDIR.${MK_OPENMP}+=	libomp
.endif
.if ${MK_USB} != "no"
SUBDIR.${MK_OPENSSH}+=	libcbor libfido2
.endif
SUBDIR.${MK_OPENSSL}+=	libmp
SUBDIR.${MK_PF}+=	libpfctl
SUBDIR.${MK_PMC}+=	libpmc libpmcstat
SUBDIR.${MK_RADIUS_SUPPORT}+=	libradius
SUBDIR.${MK_SENDMAIL}+=	libmilter libsm libsmdb libsmutil
SUBDIR.${MK_TELNET}+=	libtelnet
SUBDIR.${MK_TESTS_SUPPORT}+=	atf
SUBDIR.${MK_TESTS_SUPPORT}+=	liblutok
SUBDIR.${MK_TESTS}+=	tests
SUBDIR.${MK_UNBOUND}+=	libunbound
SUBDIR.${MK_USB}+=	libusbhid libusb
SUBDIR.${MK_OFED}+=	ofed
SUBDIR.${MK_VERIEXEC}+=	libveriexec
SUBDIR.${MK_ZFS}+=	libbe

.if !make(install)
SUBDIR_PARALLEL=
.endif

.if ${MACHINE_ABI:Mpurecap}
SUBDIR+=	libmalloc_simple
.endif

.include <bsd.subdir.mk>
