/*
 * Copyright (c) 2014 The FreeBSD Foundation.
 *
 * Portions of this software were developed by Konstantin Belousov
 * under sponsorship from the FreeBSD Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice(s), this list of conditions and the following disclaimer as
 *    the first lines of this file unmodified other than the possible
 *    addition of one or more copyright notices.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice(s), this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * CHERI CHANGES START
 * {
 *   "updated": 20221129,
 *   "target_type": "lib",
 *   "changes": [
 *     "calling_convention"
 *   ]
 * }
 * CHERI CHANGES END
 */

#include <sys/types.h>
#include <sys/fcntl.h>
#include <stdarg.h>
#include "libc_private.h"

#ifndef __CHERI_PURE_CAPABILITY__
__weak_reference(__sys_open, __open);
#else
__weak_reference(_open, __open);
#endif

#pragma weak open
int
open(const char *path, int flags, ...)
{
	va_list ap;
	int mode;

	if ((flags & O_CREAT) != 0) {
		va_start(ap, flags);
		mode = va_arg(ap, int);
		va_end(ap);
	} else {
		mode = 0;
	}
	return (INTERPOS_SYS(openat, AT_FDCWD, path, flags, mode));
}

#ifdef __CHERI_PURE_CAPABILITY__
int _open(const char *path, int flags, ...);
#pragma weak _open
int
_open(const char *path, int flags, ...)
{
	va_list ap;
	int mode;

	if ((flags & O_CREAT) != 0) {
		va_start(ap, flags);
		mode = va_arg(ap, int);
		va_end(ap);
	} else {
		mode = 0;
	}
	return (__sys_openat(AT_FDCWD, path, flags, mode));
}
#endif
