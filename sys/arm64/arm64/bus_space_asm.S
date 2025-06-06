/*-
 * Copyright (c) 2014 Andrew Turner
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
#include <machine/asm.h>

ENTRY(generic_bs_r_1)
	ldrb	w0, [PTR(1), x2]
	RETURN
END(generic_bs_r_1)

ENTRY(generic_bs_r_2)
	ldrh	w0, [PTR(1), x2]
	RETURN
END(generic_bs_r_2)

ENTRY(generic_bs_r_4)
	ldr	w0, [PTR(1), x2]
	RETURN
END(generic_bs_r_4)

ENTRY(generic_bs_r_8)
	ldr	x0, [PTR(1), x2]
	RETURN
END(generic_bs_r_8)

ENTRY(generic_bs_rm_1)
	/* If there is anything to read. */
	cbz	x4, 2f

	/* Calculate the device address. */
	add	x0, x1, x2
	/*
	 * x/c0 = The device address.
	 * x/c3 = The kernel address.
	 * x4 = Count
	 */

	/* Read the data. */
1:	ldrb	w1, [PTR(0)]
	strb	w1, [PTR(3)], #1
	subs	x4, x4, #1
	b.ne	1b

2:	RETURN
END(generic_bs_rm_1)

ENTRY(generic_bs_rm_2)
	/* If there is anything to read. */
	cbz	x4, 2f

	/* Calculate the device address. */
	add	x0, x1, x2
	/*
	 * x/c0 = The device address.
	 * x/c3 = The kernel address.
	 * x4 = Count
	 */

	/* Read the data. */
1:	ldrh	w1, [PTR(0)]
	strh	w1, [PTR(3)], #2
	subs	x4, x4, #1
	b.ne	1b

2:	RETURN
END(generic_bs_rm_2)

ENTRY(generic_bs_rm_4)
	/* If there is anything to read. */
	cbz	x4, 2f

	/* Calculate the device address. */
	add	x0, x1, x2
	/*
	 * x/c0 = The device address.
	 * x/c3 = The kernel address.
	 * x4 = Count
	 */

	/* Read the data. */
1:	ldr	w1, [PTR(0)]
	str	w1, [PTR(3)], #4
	subs	x4, x4, #1
	b.ne	1b

2:	RETURN
END(generic_bs_rm_4)

ENTRY(generic_bs_rm_8)
	/* If there is anything to read. */
	cbz	x4, 2f

	/* Calculate the device address. */
	add	x0, x1, x2
	/*
	 * x/c0 = The device address.
	 * x/c3 = The kernel address.
	 * x4 = Count
	 */

	/* Read the data. */
1:	ldr	x1, [PTR(0)]
	str	x1, [PTR(3)], #8
	subs	x4, x4, #1
	b.ne	1b

2:	RETURN
END(generic_bs_rm_8)

ENTRY(generic_bs_rr_1)
	/* Is there is anything to read. */
	cbz	x4, 2f

	/* Calculate the device address. */
	add	x0, x1, x2
	/*
	 * x/c0 = The device address.
	 * x/c3 = The kernel address.
	 * x/c4 = Count
	 */

	/* Read the data. */
1:	ldrb	w1, [PTR(0)], #1
	strb	w1, [PTR(3)], #1
	subs	x4, x4, #1
	b.ne	1b

2:	RETURN
END(generic_bs_rr_1)

ENTRY(generic_bs_rr_2)
	/* Is there is anything to read. */
	cbz	x4, 2f

	/* Calculate the device address. */
	add	x0, x1, x2
	/*
	 * x/c0 = The device address.
	 * x/c3 = The kernel address.
	 * x4 = Count
	 */

	/* Read the data. */
1:	ldrh	w1, [PTR(0)], #2
	strh	w1, [PTR(3)], #2
	subs	x4, x4, #1
	b.ne	1b

2:	RETURN
END(generic_bs_rr_2)

ENTRY(generic_bs_rr_4)
	/* Is there is anything to read. */
	cbz	x4, 2f

	/* Calculate the device address. */
	add	x0, x1, x2
	/*
	 * x/c0 = The device address.
	 * x/c3 = The kernel address.
	 * x4 = Count
	 */

	/* Read the data. */
1:	ldr	w1, [PTR(0)], #4
	str	w1, [PTR(3)], #4
	subs	x4, x4, #1
	b.ne	1b

2:	RETURN
END(generic_bs_rr_4)

ENTRY(generic_bs_rr_8)
	/* Is there is anything to read. */
	cbz	x4, 2f

	/* Calculate the device address. */
	add	x0, x1, x2
	/*
	 * x/c0 = The device address.
	 * x/c3 = The kernel address.
	 * x4 = Count
	 */

	/* Read the data. */
1:	ldr	x1, [PTR(0)], #8
	str	x1, [PTR(3)], #8
	subs	x4, x4, #1
	b.ne	1b

2:	RETURN
END(generic_bs_rr_8)


ENTRY(generic_bs_w_1)
	strb	w3, [PTR(1), x2]
	RETURN
END(generic_bs_w_1)

ENTRY(generic_bs_w_2)
	strh	w3, [PTR(1), x2]
	RETURN
END(generic_bs_w_2)

ENTRY(generic_bs_w_4)
	str	w3, [PTR(1), x2]
	RETURN
END(generic_bs_w_4)

ENTRY(generic_bs_w_8)
	str	x3, [PTR(1), x2]
	RETURN
END(generic_bs_w_8)

ENTRY(generic_bs_wm_1)
	/* If there is anything to write. */
	cbz	x4, 2f

	add	x0, x1, x2
	/*
	 * x/c0 = The device address.
	 * x/c3 = The kernel address.
	 * x4 = Count
	 */

	/* Write the data */
1:	ldrb	w1, [PTR(3)], #1
	strb	w1, [PTR(0)]
	subs	x4, x4, #1
	b.ne	1b

2:	RETURN
END(generic_bs_wm_1)

ENTRY(generic_bs_wm_2)
	/* If there is anything to write. */
	cbz	x4, 2f

	add	x0, x1, x2
	/*
	 * x/c0 = The device address.
	 * x/c3 = The kernel address.
	 * x4 = Count
	 */

	/* Write the data */
1:	ldrh	w1, [PTR(3)], #2
	strh	w1, [PTR(0)]
	subs	x4, x4, #1
	b.ne	1b

2:	RETURN
END(generic_bs_wm_2)

ENTRY(generic_bs_wm_4)
	/* If there is anything to write. */
	cbz	x4, 2f

	add	x0, x1, x2
	/*
	 * x/c0 = The device address.
	 * x/c3 = The kernel address.
	 * x4 = Count
	 */

	/* Write the data */
1:	ldr	w1, [PTR(3)], #4
	str	w1, [PTR(0)]
	subs	x4, x4, #1
	b.ne	1b

2:	RETURN
END(generic_bs_wm_4)

ENTRY(generic_bs_wm_8)
	/* If there is anything to write. */
	cbz	x4, 2f

	add	x0, x1, x2
	/*
	 * x/c0 = The device address.
	 * x/c3 = The kernel address.
	 * x4 = Count
	 */

	/* Write the data */
1:	ldr	x1, [PTR(3)], #8
	str	x1, [PTR(0)]
	subs	x4, x4, #1
	b.ne	1b

2:	RETURN
END(generic_bs_wm_8)

ENTRY(generic_bs_wr_1)
	/* Is there is anything to write. */
	cbz	x4, 2f
	
	add	x0, x1, x2
	/*
	 * x/c0 = The device address.
	 * x/c3 = The kernel address.
	 * x4 = Count
	 */

	/* Write the data */
1:	ldrb	w1, [PTR(3)], #1
	strb	w1, [PTR(0)], #1
	subs	x4, x4, #1
	b.ne	1b

2:	RETURN
END(generic_bs_wr_1)

ENTRY(generic_bs_wr_2)
	/* Is there is anything to write. */
	cbz	x4, 2f
	
	add	x0, x1, x2
	/*
	 * x/c0 = The device address.
	 * x/c3 = The kernel address.
	 * x4 = Count
	 */

	/* Write the data */
1:	ldrh	w1, [PTR(3)], #2
	strh	w1, [PTR(0)], #2
	subs	x4, x4, #1
	b.ne	1b

2:	RETURN
END(generic_bs_wr_2)

ENTRY(generic_bs_wr_4)
	/* Is there is anything to write. */
	cbz	x4, 2f
	
	add	x0, x1, x2
	/*
	 * x/c0 = The device address.
	 * x/c3 = The kernel address.
	 * x4 = Count
	 */

	/* Write the data */
1:	ldr	w1, [PTR(3)], #4
	str	w1, [PTR(0)], #4
	subs	x4, x4, #1
	b.ne	1b

2:	RETURN
END(generic_bs_wr_4)

ENTRY(generic_bs_wr_8)
	/* Is there is anything to write. */
	cbz	x4, 2f
	
	add	x0, x1, x2
	/*
	 * x/c0 = The device address.
	 * x/c3 = The kernel address.
	 * x4 = Count
	 */

	/* Write the data */
1:	ldr	x1, [PTR(3)], #8
	str	x1, [PTR(0)], #8
	subs	x4, x4, #1
	b.ne	1b

2:	RETURN
END(generic_bs_wr_8)

ENTRY(generic_bs_fault)
	mov	x0, #-1
	RETURN
END(generic_bs_fault)

ENTRY(generic_bs_peek_1)
	.globl	generic_bs_peek_1f
generic_bs_peek_1f:
	ldrb	w0, [PTR(1), x2]	/* Checked instruction */
	dsb	sy
	strb	w0,[PTR(3)]
	mov	x0, #0
	RETURN
END(generic_bs_peek_1)

	.section .rodata
	.globl generic_bs_peek_1_fault
generic_bs_peek_1_fault:
	.quad	generic_bs_peek_1f

ENTRY(generic_bs_peek_2)
	.globl	generic_bs_peek_2f
generic_bs_peek_2f:
	ldrh	w0, [PTR(1), x2]	/* Checked instruction */
	dsb	sy
	strh	w0,[PTR(3)]
	mov	x0, #0
	RETURN
END(generic_bs_peek_2)

ENTRY(generic_bs_peek_4)
	.globl	generic_bs_peek_4f
generic_bs_peek_4f:
	ldr	w0, [PTR(1), x2]	/* Checked instruction */
	dsb	sy
	str	w0,[PTR(3)]
	mov	x0, #0
	RETURN
END(generic_bs_peek_4)

ENTRY(generic_bs_peek_8)
	.globl	generic_bs_peek_8f
generic_bs_peek_8f:
	ldr	x0, [PTR(1), x2]	/* Checked instruction */
	dsb	sy
	str	x0,[PTR(3)]
	mov	x0, #0
	RETURN
END(generic_bs_peek_8)

ENTRY(generic_bs_poke_1)
	.globl	generic_bs_poke_1f
generic_bs_poke_1f:
	strb	w3, [PTR(1), x2]	/* Checked instruction */
	dsb	sy
	mov	x0, #0
	RETURN
END(generic_bs_poke_1)

ENTRY(generic_bs_poke_2)
	.globl	generic_bs_poke_2f
generic_bs_poke_2f:
	strh	w3, [PTR(1), x2]	/* Checked instruction */
	dsb	sy
	mov	x0, #0
	RETURN
END(generic_bs_poke_2)

ENTRY(generic_bs_poke_4)
	.globl	generic_bs_poke_4f
generic_bs_poke_4f:
	str	w3, [PTR(1), x2]	/* Checked instruction */
	dsb	sy
	mov	x0, #0
	RETURN
END(generic_bs_poke_4)

ENTRY(generic_bs_poke_8)
	.globl	generic_bs_poke_8f
generic_bs_poke_8f:
	str	x3, [PTR(1), x2]	/* Checked instruction */
	dsb	sy
	mov	x0, #0
	RETURN
END(generic_bs_poke_8)

GNU_PROPERTY_AARCH64_FEATURE_1_NOTE(GNU_PROPERTY_AARCH64_FEATURE_1_VAL)
