#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * clib: tiny pure-C library that lives at the bottom of the dep graph.
 *
 * BUILD-SYSTEM PURPOSE:
 *   - Exercises the C/C++ boundary in Bazel (cc_library with .c sources).
 *   - Used by libs/crypto, which adds -ldl / -lpthread linkopts.
 *   - In cross-compile (--config=aarch64), Bazel must find these
 *     symbols in the sysroot, not the host libc — that's the Day 6 exercise.
 */

/* CRC-32 (IEEE 802.3 polynomial, bitwise implementation). */
uint32_t clib_crc32(const void* data, size_t len);

/* Constant-time memory comparison (avoids timing side-channels). */
int clib_memcmp_ct(const void* a, const void* b, size_t len);

#ifdef __cplusplus
}  /* extern "C" */
#endif
