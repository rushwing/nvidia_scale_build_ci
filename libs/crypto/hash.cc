#include "libs/crypto/hash.h"

#include <cstdio>
#include <cstring>

#if defined(__linux__)
#  include <dlfcn.h>   // triggers -ldl linkopt (Day 6: must exist in sysroot)
#  include <pthread.h> // triggers -lpthread linkopt
#endif

#include "libs/ffi/clib.h"

namespace nvidia::crypto {

// ── Optional hardware-acceleration probe (Linux only) ────────────────────────
// In a real NVIDIA library, this would probe for libcuda or libcrypto.
// Here it demonstrates: (1) runtime dynamic loading, (2) why -ldl is needed,
// (3) why cross-compiling to aarch64 requires the sysroot to contain libdl.so.
#if defined(__linux__)
namespace {
bool probe_hw_accel() {
  // RTLD_LAZY | RTLD_NOLOAD: check if already loaded; don't dlopen if absent.
  void* lib = dlopen("libssl.so.3", RTLD_LAZY | RTLD_NOLOAD);
  if (!lib) lib = dlopen("libssl.so.1.1", RTLD_LAZY | RTLD_NOLOAD);
  if (lib) {
    dlclose(lib);
    return true;
  }
  return false;
}
const bool kHasHwAccel = probe_hw_accel();
}  // namespace
#endif

// ── FNV-1a 64-bit ────────────────────────────────────────────────────────────
uint64_t fnv1a_64(const void* data, size_t len) {
  constexpr uint64_t kPrime  = 0x00000100000001B3ULL;
  constexpr uint64_t kOffset = 0xcbf29ce484222325ULL;
  uint64_t hash = kOffset;
  const auto* ptr = static_cast<const uint8_t*>(data);
  for (size_t i = 0; i < len; ++i) {
    hash ^= static_cast<uint64_t>(ptr[i]);
    hash *= kPrime;
  }
  return hash;
}

// ── sha256_hex (stub) ────────────────────────────────────────────────────────
// Returns FNV-1a as 16-char hex — good enough to demonstrate content-addressing
// in the build system demo. Replace with OpenSSL for real security work.
std::string sha256_hex(const std::vector<uint8_t>& data) {
  const uint64_t h = fnv1a_64(data.data(), data.size());
  char buf[17];
  std::snprintf(buf, sizeof(buf), "%016llx",
                static_cast<unsigned long long>(h));
  return std::string(buf);
}

// ── crc32 ─────────────────────────────────────────────────────────────────────
uint32_t crc32(const std::vector<uint8_t>& data) {
  return clib_crc32(data.data(), data.size());
}

}  // namespace nvidia::crypto
