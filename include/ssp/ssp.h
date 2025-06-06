/*	$NetBSD: ssp.h,v 1.13 2015/09/03 20:43:47 plunky Exp $	*/

/*-
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2006, 2011 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Christos Zoulas.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _SSP_SSP_H_
#define _SSP_SSP_H_

#include <sys/cdefs.h>

#if !defined(__cplusplus) && !defined(__CHERI_PURE_CAPABILITY__)
# if defined(_FORTIFY_SOURCE) && _FORTIFY_SOURCE > 0 && \
     (__OPTIMIZE__ > 0 || defined(__clang__))
#  if _FORTIFY_SOURCE > 1
#   define __SSP_FORTIFY_LEVEL 2
#  else
#   define __SSP_FORTIFY_LEVEL 1
#  endif
# else
#  define __SSP_FORTIFY_LEVEL 0
# endif
#else
# define __SSP_FORTIFY_LEVEL 0
#endif

#define	__ssp_var(type)	__CONCAT(__ssp_ ## type, __COUNTER__)

/* __ssp_real is used by the implementation in libc */
#if __SSP_FORTIFY_LEVEL == 0
#define __ssp_real_(fun)	fun
#else
#define __ssp_real_(fun)	__ssp_real_ ## fun
#endif
#define __ssp_real(fun)		__ssp_real_(fun)

#define __ssp_inline static __inline __attribute__((__always_inline__))

#define __ssp_bos(ptr) __builtin_object_size(ptr, __SSP_FORTIFY_LEVEL > 1)
#define __ssp_bos0(ptr) __builtin_object_size(ptr, 0)

#define __ssp_check(buf, len, bos) \
	if (bos(buf) != (size_t)-1 && (size_t)len > bos(buf)) \
		__chk_fail()

#define __ssp_redirect_raw_impl(rtype, fun, symbol, args) \
rtype __ssp_real_(fun) args __RENAME(symbol); \
__ssp_inline rtype fun args __RENAME(__ssp_protected_ ## fun); \
__ssp_inline rtype fun args

#define __ssp_redirect_raw(rtype, fun, symbol, args, call, cond, bos, len) \
__ssp_redirect_raw_impl(rtype, fun, symbol, args) { \
	if (cond) \
		__ssp_check(__buf, len, bos); \
	return __ssp_real_(fun) call; \
}

#define __ssp_redirect(rtype, fun, args, call) \
    __ssp_redirect_raw(rtype, fun, fun, args, call, 1, __ssp_bos, __len)
#define __ssp_redirect0(rtype, fun, args, call) \
    __ssp_redirect_raw(rtype, fun, fun, args, call, 1, __ssp_bos0, __len)

#include <machine/_limits.h>

__ssp_inline int
__ssp_overlap(const void *leftp, const void *rightp, __size_t sz)
{
	__uintptr_t left = (__uintptr_t)leftp;
	__uintptr_t right = (__uintptr_t)rightp;

	if (left <= right)
		return (__SIZE_T_MAX - sz < left || right < left + sz);

	return (__SIZE_T_MAX - sz < right || left < right + sz);
}

#include <sys/_iovec.h>

__BEGIN_DECLS
void __stack_chk_fail(void) __dead2;
void __chk_fail(void) __dead2;
__END_DECLS

__ssp_inline void
__ssp_check_iovec(const struct iovec *iov, int iovcnt)
{
	const size_t iovsz = __ssp_bos(iov);
	int i;

	if (iovsz != (size_t)-1 && iovsz / sizeof(*iov) < (size_t)iovcnt)
		__chk_fail();

	for (i = 0; i < iovcnt; i++) {
		if (__ssp_bos(iov[i].iov_base) < iov[i].iov_len)
			__chk_fail();
	}
}

#endif /* _SSP_SSP_H_ */
