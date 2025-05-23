/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 1982, 1986, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
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
 */

#ifndef _SYS__IOVEC_H_
#define	_SYS__IOVEC_H_

#include <sys/_types.h>

#ifndef _SIZE_T_DECLARED
typedef	__size_t	size_t;
#define	_SIZE_T_DECLARED
#endif

struct iovec {
	void * __kerncap	iov_base;	/* Base address. */
	size_t			iov_len;	/* Length. */
};

#ifdef _KERNEL
#ifdef __CHERI_PURE_CAPABILITY__
#define	IOVEC_INIT IOVEC_INIT_C
#else /* ! __CHERI_PURE_CAPABILITY__ */
#define	IOVEC_INIT(iovp, base, len)	do {				\
	(iovp)->iov_base = PTR2CAP((base));				\
	(iovp)->iov_len = (len);					\
} while(0)
#endif /* ! __CHERI_PURE_CAPABILITY__ */
#define IOVEC_INIT_C(iovp, base, len)	do {				\
	(iovp)->iov_base = (base);					\
	(iovp)->iov_len = (len);					\
} while(0)
#else
#define IOVEC_INIT(iovp, base, len)	do {				\
	(iovp)->iov_base = (base);					\
	(iovp)->iov_len = (len);					\
} while(0)
#define	IOVEC_INIT_C IOVEC_INIT
#endif /* _KERNEL */

/* String with length including NUL terminator */
#define	IOVEC_INIT_CSTR(iovp, str)	do {				\
	void *__str = (str);						\
	IOVEC_INIT(iovp, __str, strlen(__str) + 1);			\
} while(0)

/* XXX: CheriBSD compat */
#define	IOVEC_INIT_STR		IOVEC_INIT_CSTR

/* Object with size from sizeof() */
#define	IOVEC_INIT_OBJ(iovp, obj)					\
	IOVEC_INIT(iovp, &(obj), sizeof(obj))

#define	IOVEC_ADVANCE(iovp, amt)	do {				\
	struct iovec *__iovp = (iovp);					\
	size_t __amt = (amt);						\
	KASSERT(__amt <= __iovp->iov_len, ("%s: amount %zu > iov_len	\
	    %zu", __func__, __amt, __iovp->iov_len));			\
	__iovp->iov_len -= __amt;					\
	__iovp->iov_base = (char * __capability)__iovp->iov_base + __amt; \
} while(0)

#ifdef _KERNEL
struct uio;

typedef int(copyiniov_t)(const struct iovec * __capability iovp, unsigned int iovcnt,
    struct iovec **iov, int error);
typedef int(copyinuio_t)(const struct iovec * __capability iovp, unsigned int iovcnt,
    struct uio **iov);
typedef int(updateiov_t)(const struct uio *uiop,
    struct iovec * __capability iovp);
#endif

#endif /* !_SYS__IOVEC_H_ */
// CHERI CHANGES START
// {
//   "updated": 20230509,
//   "target_type": "header",
//   "changes": [
//     "iovec-macros"
//   ]
// }
// CHERI CHANGES END
