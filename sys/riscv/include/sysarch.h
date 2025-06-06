/*-
 * Copyright (c) 1993 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	from: FreeBSD: src/sys/i386/include/sysarch.h,v 1.14 2000/09/21
 */

/*
 * Architecture specific syscalls (RISC-V)
 */
#ifndef _MACHINE_SYSARCH_H_
#define	_MACHINE_SYSARCH_H_

/*
 * Query, enable, and disable QEMU ISA-level tracing on threads. To use this
 * feature, the sysctl hw.qemu_trace_perthread must be enabled.
 */
#define	QEMU_GET_QTRACE		100	/* Get QEMU tracing. */
#define	QEMU_SET_QTRACE		101	/* Set (or clear) QEMU tracing. */
#define	QEMU_SET_QTRACE_USER	102	/* Set (or clear) QEMU per-thread user-only tracing mode */

#ifndef _KERNEL

__BEGIN_DECLS
int	sysarch(int _number, void *_args);
__END_DECLS

#endif

#endif /* !_MACHINE_SYSARCH_H_ */
