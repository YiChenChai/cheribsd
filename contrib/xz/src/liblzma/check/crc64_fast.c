// SPDX-License-Identifier: 0BSD

///////////////////////////////////////////////////////////////////////////////
//
/// \file       crc64.c
/// \brief      CRC64 calculation
//
//  Authors:    Lasse Collin
//              Ilya Kurdyukov
//
///////////////////////////////////////////////////////////////////////////////
/*
 * CHERI CHANGES START
 * {
 *   "updated": 20221129,
 *   "target_type": "lib",
 *   "changes": [
 *     "pointer_alignment"
 *   ]
 * }
 * CHERI CHANGES END
 */
#include "check.h"
#include "crc_common.h"

#if defined(CRC_X86_CLMUL)
#	define BUILDING_CRC64_CLMUL
#	include "crc_x86_clmul.h"
#endif


#ifdef CRC64_GENERIC

/////////////////////////////////
// Generic slice-by-four CRC64 //
/////////////////////////////////

#ifdef WORDS_BIGENDIAN
#	define A1(x) ((x) >> 56)
#else
#	define A1 A
#endif


// See the comments in crc32_fast.c. They aren't duplicated here.
static uint64_t
crc64_generic(const uint8_t *buf, size_t size, uint64_t crc)
{
	crc = ~crc;

#ifdef WORDS_BIGENDIAN
	crc = byteswap64(crc);
#endif

	if (size > 4) {
		while ((size_t)(buf) & 3) {
			crc = lzma_crc64_table[0][*buf++ ^ A1(crc)] ^ S8(crc);
			--size;
		}

		const uint8_t *const limit = buf + (size & ~(size_t)(3));
		size &= (size_t)(3);

		while (buf < limit) {
#ifdef WORDS_BIGENDIAN
			const uint32_t tmp = (uint32_t)(crc >> 32)
					^ aligned_read32ne(buf);
#else
			const uint32_t tmp = (uint32_t)crc
					^ aligned_read32ne(buf);
#endif
			buf += 4;

			crc = lzma_crc64_table[3][A(tmp)]
			    ^ lzma_crc64_table[2][B(tmp)]
			    ^ S32(crc)
			    ^ lzma_crc64_table[1][C(tmp)]
			    ^ lzma_crc64_table[0][D(tmp)];
		}
	}

	while (size-- != 0)
		crc = lzma_crc64_table[0][*buf++ ^ A1(crc)] ^ S8(crc);

#ifdef WORDS_BIGENDIAN
	crc = byteswap64(crc);
#endif

	return ~crc;
}
#endif


#if defined(CRC64_GENERIC) && defined(CRC64_ARCH_OPTIMIZED)

//////////////////////////
// Function dispatching //
//////////////////////////

// If both the generic and arch-optimized implementations are usable, then
// the function that is used is selected at runtime. See crc32_fast.c.

typedef uint64_t (*crc64_func_type)(
		const uint8_t *buf, size_t size, uint64_t crc);

static crc64_func_type
crc64_resolve(void)
{
	return is_arch_extension_supported()
			? &crc64_arch_optimized : &crc64_generic;
}

#ifdef HAVE_FUNC_ATTRIBUTE_CONSTRUCTOR
#	define CRC64_SET_FUNC_ATTR __attribute__((__constructor__))
static crc64_func_type crc64_func;
#else
#	define CRC64_SET_FUNC_ATTR
static uint64_t crc64_dispatch(const uint8_t *buf, size_t size, uint64_t crc);
static crc64_func_type crc64_func = &crc64_dispatch;
#endif


CRC64_SET_FUNC_ATTR
static void
crc64_set_func(void)
{
	crc64_func = crc64_resolve();
	return;
}


#ifndef HAVE_FUNC_ATTRIBUTE_CONSTRUCTOR
static uint64_t
crc64_dispatch(const uint8_t *buf, size_t size, uint64_t crc)
{
	crc64_set_func();
	return crc64_func(buf, size, crc);
}
#endif
#endif


extern LZMA_API(uint64_t)
lzma_crc64(const uint8_t *buf, size_t size, uint64_t crc)
{
#if defined(CRC64_GENERIC) && defined(CRC64_ARCH_OPTIMIZED)

#ifdef CRC_USE_GENERIC_FOR_SMALL_INPUTS
	if (size <= 16)
		return crc64_generic(buf, size, crc);
#endif
	return crc64_func(buf, size, crc);

#elif defined(CRC64_ARCH_OPTIMIZED)
	// If arch-optimized version is used unconditionally without runtime
	// CPU detection then omitting the generic version and its 8 KiB
	// lookup table makes the library smaller.
	//
	// FIXME: Lookup table isn't currently omitted on 32-bit x86,
	// see crc64_table.c.
	return crc64_arch_optimized(buf, size, crc);

#else
	return crc64_generic(buf, size, crc);
#endif
}
