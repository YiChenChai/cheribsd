/*-
 * Copyright (c) 2010 Isilon Systems, Inc.
 * Copyright (c) 2010 iX Systems, Inc.
 * Copyright (c) 2010 Panasas, Inc.
 * Copyright (c) 2013-2016 Mellanox Technologies, Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#ifndef __DRMCOMPAT_LINUX_PAGE_H__
#define	__DRMCOMPAT_LINUX_PAGE_H__

#include <linux/types.h>

#include <sys/param.h>
#include <sys/vmmeter.h>

#include <machine/atomic.h>
#include <vm/vm.h>
#include <vm/vm_page.h>
#include <vm/pmap.h>

typedef unsigned long linux_pte_t;
typedef unsigned long linux_pmd_t;
typedef unsigned long linux_pgd_t;
typedef unsigned long pgprot_t;

#define page	vm_page

#define	DRMCOMPAT_PROT_VALID (1 << 6)
#define	DRMCOMPAT_CACHE_MODE_SHIFT 7

CTASSERT((VM_PROT_ALL & -DRMCOMPAT_PROT_VALID) == 0);

static inline pgprot_t
cachemode2protval(vm_memattr_t attr)
{
	return ((attr << DRMCOMPAT_CACHE_MODE_SHIFT) | DRMCOMPAT_PROT_VALID);
}

static inline vm_memattr_t
pgprot2cachemode(pgprot_t prot)
{
	if (prot & DRMCOMPAT_PROT_VALID)
		return (prot >> DRMCOMPAT_CACHE_MODE_SHIFT);
	else
		return (VM_MEMATTR_DEFAULT);
}

#define	virt_to_page(x)		PHYS_TO_VM_PAGE(vtophys(x))
#define	page_to_pfn(pp)		(VM_PAGE_TO_PHYS(pp) >> PAGE_SHIFT)
#define	pfn_to_page(pfn)	(PHYS_TO_VM_PAGE((pfn) << PAGE_SHIFT))
#define	nth_page(page,n)	pfn_to_page(page_to_pfn(page) + (n))

#define	clear_page(page)		memset(page, 0, PAGE_SIZE)
#define	pgprot_noncached(prot)		\
	(((prot) & VM_PROT_ALL) | cachemode2protval(VM_MEMATTR_UNCACHEABLE))
#define	pgprot_writecombine(prot)	\
	(((prot) & VM_PROT_ALL) | cachemode2protval(VM_MEMATTR_WRITE_COMBINING))

#undef	PAGE_MASK
#define	PAGE_MASK	(~(PAGE_SIZE-1))
/*
 * Modifying PAGE_MASK in the above way breaks trunc_page, round_page,
 * and btoc macros. Therefore, redefine them in a way that makes sense
 * so the DRMCOMPAT consumers don't get totally broken behavior.
 */
#undef	btoc
#define	btoc(x)	(((vm_offset_t)(x) + PAGE_SIZE - 1) >> PAGE_SHIFT)
#undef	round_page
#define	round_page(x)	((((uintptr_t)(x)) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#undef	trunc_page
#define	trunc_page(x)	((uintptr_t)(x) & ~(PAGE_SIZE - 1))


#define PAGE_KERNEL_IO  0x0000

/* XXX note that this is incomplete */
void *kmap(vm_page_t page);
void *kmap_atomic(vm_page_t page);
void *kmap_atomic_prot(vm_page_t page, pgprot_t prot);
void kunmap(vm_page_t page);
void kunmap_atomic(void *vaddr);

void iounmap_atomic(void *vaddr);

static inline int
page_count(vm_page_t page __unused)
{
	return (1);
}

int set_pages_array_wb(struct page **pages, int addrinarray);
int set_pages_array_uc(struct page **pages, int addrinarray);
int set_pages_array_wc(struct page **pages, int addrinarray);

int set_pages_wb(vm_page_t page, int numpages);
int set_pages_uc(vm_page_t page, int numpages);
int set_pages_wc(vm_page_t page, int numpages);

vm_paddr_t page_to_phys(vm_page_t page);

void unmap_mapping_range(void *obj, loff_t const holebegin,
    loff_t const holelen, int even_cows);

#define linux_clflushopt(arg) __linux_clflushopt((u_long)(arg))
extern void __linux_clflushopt(u_long addr);

#endif	/* __DRMCOMPAT_LINUX_PAGE_H__ */
