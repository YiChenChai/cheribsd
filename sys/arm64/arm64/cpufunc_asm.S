/*-
 * Copyright (c) 2014 Robin Randhawa
 * Copyright (c) 2015 The FreeBSD Foundation
 * All rights reserved.
 *
 * Portions of this software were developed by Andrew Turner
 * under sponsorship from the FreeBSD Foundation
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <sys/elf_common.h>
#include <sys/errno.h>

#include <machine/asm.h>
#include <machine/param.h>

#include "assym.inc"
/*
 * FIXME:
 * Need big.LITTLE awareness at some point.
 * Using arm64_p[id]cache_line_size may not be the best option.
 * Need better SMP awareness.
 */
	.text
	.align	2

.Lpage_mask:
	.word	PAGE_MASK

/*
 * Macro to handle the cache. This takes the start address in x/c0, length
 * in x1. It will corrupt x0, x1, x2, x3, x4 and x6.
 */
.macro cache_handle_range dcop = 0, ic = 0, icop = 0
#ifdef __CHERI_PURE_CAPABILITY__
	/* Ensure that the capability allows access to the range
	 * [address, address + x1)
	 */
	scbnds	c6, c0, x1
#endif
.if \ic == 0
	/* Load the D cache line size */
	LDR_LABEL(x3, PTR(3), dcache_line_size)
.else
	/* Load the I & D cache line size */
	LDR_LABEL(x3, PTR(3), idcache_line_size)
.endif
	sub	x4, x3, #1		/* Get the address mask */
	and	x2, x0, x4		/* Get the low bits of the address */
	add	x1, x1, x2		/* Add these to the size */
	bic	x0, x0, x4		/* Clear the low bit of the address */
#ifdef __CHERI_PURE_CAPABILITY__
	/*
	 * There is no guarantee that the capability given here has
	 * sufficient bounds for the whole cache line.
	 * Re-derive the capability from the kernel root if the addr capability
	 * is valid within the target range of cache lines, otherwise strip the
	 * tag and let the dc/ic instruction fail.
	 */
	LDR_LABEL(c2, c2, kernel_root_cap)
	scvalue c0, c2, x0
	add	x2, x1, x3
	bic	x2, x2, x4
	scbnds	c0, c0, x2
	chkss	c6, c0
	b.mi	3f
	/* Use c6 as the source here for more informative debug info */
	clrtag	c0, c6
3:
#endif
.if \ic != 0
	mov	PTR(2), PTR(0)		/* Save the address */
	mov	x4, x1			/* Save the size */
.endif
1:
	dc	\dcop, PTR(0)
	add	PTR(0), PTR(0), x3	/* Move to the next line */
	subs	x1, x1, x3		/* Reduce the size */
	b.hi	1b			/* Check if we are done */
	dsb	ish
.if \ic != 0
2:
	ic	\icop, PTR(2)
	add	PTR(2), PTR(2), x3	/* Move to the next line */
	subs	x4, x4, x3		/* Reduce the size */
	b.hi	2b			/* Check if we are done */
	dsb	ish
	isb
.endif
.endm

ENTRY(arm64_nullop)
	RETURN
END(arm64_nullop)

/*
 * Generic functions to read/modify/write the internal coprocessor registers
 */

ENTRY(arm64_tlb_flushID)
	dsb	ishst
#ifdef SMP
	tlbi	vmalle1is
#else
	tlbi	vmalle1
#endif
	dsb	ish
	isb
	RETURN
END(arm64_tlb_flushID)

/*
 * void arm64_dcache_wb_range(void *, vm_size_t)
 */
ENTRY(arm64_dcache_wb_range)
	cache_handle_range	dcop = cvac
	RETURN
END(arm64_dcache_wb_range)

/*
 * void arm64_dcache_wbinv_range(void *, vm_size_t)
 */
ENTRY(arm64_dcache_wbinv_range)
	cache_handle_range	dcop = civac
	RETURN
END(arm64_dcache_wbinv_range)

/*
 * void arm64_dcache_inv_range(void *, vm_size_t)
 *
 * Note, we must not invalidate everything.  If the range is too big we
 * must use wb-inv of the entire cache.
 */
ENTRY(arm64_dcache_inv_range)
	cache_handle_range	dcop = ivac
	RETURN
END(arm64_dcache_inv_range)

/*
 * void arm64_dic_idc_icache_sync_range(void *, vm_size_t)
 * When the CTR_EL0.IDC bit is set cleaning to PoU becomes a dsb.
 * When the CTR_EL0.DIC bit is set icache invalidation becomes an isb.
 */
ENTRY(arm64_dic_idc_icache_sync_range)
	dsb	ishst
	isb
	RETURN
END(arm64_dic_idc_icache_sync_range)

/*
 * void arm64_idc_aliasing_icache_sync_range(void *, vm_size_t)
 * When the CTR_EL0.IDC bit is set cleaning to PoU becomes a dsb.
 */
ENTRY(arm64_idc_aliasing_icache_sync_range)
	dsb	ishst
	ic	ialluis
	dsb	ish
	isb
	RETURN
END(arm64_idc_aliasing_icache_sync_range)

/*
 * void arm64_aliasing_icache_sync_range(void *, vm_size_t)
 */
ENTRY(arm64_aliasing_icache_sync_range)
	/*
	 * XXX Temporary solution - I-cache flush should be range based for
	 * PIPT cache or IALLUIS for VIVT or VIPT caches
	 */
/*	cache_handle_range	dcop = cvau, ic = 1, icop = ivau */
	cache_handle_range	dcop = cvau
	ic	ialluis
	dsb	ish
	isb
	RETURN
END(arm64_aliasing_icache_sync_range)

/*
 * int arm64_icache_sync_range_checked(void *, vm_size_t)
 */
ENTRY(arm64_icache_sync_range_checked)
	adr	PTR(5), cache_maint_fault
	SET_FAULT_HANDLER(PTR(5), PTR(6))
	/* XXX: See comment in arm64_icache_sync_range */
	cache_handle_range	dcop = cvau
	ic	ialluis
	dsb	ish
	isb
	SET_FAULT_HANDLER(xzr, PTR(6))
	mov	x0, #0
	RETURN
END(arm64_icache_sync_range_checked)

ENTRY(cache_maint_fault)
	SET_FAULT_HANDLER(xzr, PTR(1))
	mov	x0, #EFAULT
	RETURN
END(cache_maint_fault)

GNU_PROPERTY_AARCH64_FEATURE_1_NOTE(GNU_PROPERTY_AARCH64_FEATURE_1_VAL)
