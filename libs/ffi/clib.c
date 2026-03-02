#include "libs/ffi/clib.h"

#include <stdint.h>

/* ── CRC-32 (bitwise, no lookup table) ──────────────────────────────────────
 * Using the reflected IEEE 802.3 polynomial 0xEDB88320.
 * Bitwise avoids a 1 KB table in the binary (relevant for small sysroots). */
uint32_t clib_crc32(const void* data, size_t len) {
  const uint8_t* p = (const uint8_t*)data;
  uint32_t crc = 0xFFFFFFFFU;
  for (size_t i = 0; i < len; ++i) {
    uint8_t byte = p[i];
    for (int bit = 0; bit < 8; ++bit) {
      uint32_t b = (crc ^ byte) & 1U;
      crc >>= 1;
      if (b) crc ^= 0xEDB88320U;
      byte >>= 1;
    }
  }
  return crc ^ 0xFFFFFFFFU;
}

/* ── Constant-time memcmp ────────────────────────────────────────────────────
 * Returns 0 if equal, non-zero otherwise.
 * The loop is not short-circuited, preventing timing attacks. */
int clib_memcmp_ct(const void* a, const void* b, size_t len) {
  const uint8_t* pa = (const uint8_t*)a;
  const uint8_t* pb = (const uint8_t*)b;
  uint8_t diff = 0;
  for (size_t i = 0; i < len; ++i) {
    diff |= pa[i] ^ pb[i];
  }
  return (int)diff;
}
