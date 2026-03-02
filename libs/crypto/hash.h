#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace nvidia::crypto {

// ── FNV-1a 64-bit hash ───────────────────────────────────────────────────────
// Fast non-cryptographic hash. Used for content-addressing artifacts in the CI.
uint64_t fnv1a_64(const void* data, size_t len);

// ── SHA-256 (hex digest) ─────────────────────────────────────────────────────
// Stub: real impl would delegate to OpenSSL/libcrypto.
// We deliberately use dlopen() at startup to check for optional hw acceleration
// (libssl), which forces a -ldl linkopt and exercises the sysroot path on Day 6.
std::string sha256_hex(const std::vector<uint8_t>& data);

// ── CRC-32 passthrough ───────────────────────────────────────────────────────
// Wraps //libs/ffi:clib to create a C++ API.
uint32_t crc32(const std::vector<uint8_t>& data);

}  // namespace nvidia::crypto
