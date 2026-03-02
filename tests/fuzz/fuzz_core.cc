#include "libs/core/tensor.h"

#include <cstdint>
#include <cstring>

// ─────────────────────────────────────────────────────────────────────────────
// libFuzzer entry point.
//
// Build + run with:
//   bazel build //tests/fuzz:fuzz_core --config=asan
//   ./bazel-bin/tests/fuzz/fuzz_core -runs=10000 -max_len=256
//
// When FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION is NOT defined (normal test),
// a standalone main is compiled instead so `bazel test` still works.
// ─────────────────────────────────────────────────────────────────────────────

static void fuzz_once(const uint8_t* data, size_t size) {
  // Need at least two 2-D shapes + some float data.
  if (size < 4 * sizeof(float) + 2 * sizeof(int32_t)) return;

  size_t offset = 0;

  // Parse two dimension values (clamped to avoid OOM in naive matmul).
  int32_t m_raw, n_raw;
  std::memcpy(&m_raw, data + offset, sizeof(int32_t)); offset += sizeof(int32_t);
  std::memcpy(&n_raw, data + offset, sizeof(int32_t)); offset += sizeof(int32_t);
  int64_t M = (std::abs(m_raw) % 8) + 1;  // 1–8
  int64_t N = (std::abs(n_raw) % 8) + 1;  // 1–8

  // Build a 1×M tensor A and M×N tensor B from remaining bytes.
  size_t floats_needed = static_cast<size_t>(M + M * N);
  size_t floats_avail  = (size - offset) / sizeof(float);
  if (floats_avail < floats_needed) return;

  std::vector<float> a_data(M), b_data(M * N);
  std::memcpy(a_data.data(), data + offset, M * sizeof(float));
  offset += M * sizeof(float);
  std::memcpy(b_data.data(), data + offset, M * N * sizeof(float));

  try {
    nvidia::core::Tensor A(a_data, {1, M});
    nvidia::core::Tensor B(b_data, {M, N});
    auto C = nvidia::core::matmul(A, B);
    auto D = nvidia::core::relu(C);
    (void)D;
  } catch (const std::invalid_argument&) {
    // Expected for malformed inputs — not a bug.
  }
}

#ifdef FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  fuzz_once(data, size);
  return 0;
}

#else

// Standalone smoke-test so `bazel test //tests/fuzz:fuzz_core` passes
// without a libFuzzer-enabled compiler.
int main() {
  const uint8_t seed[] = {
    // M=2, N=2
    0x02, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00,
    // A: [[1.0, 2.0]]
    0x00, 0x00, 0x80, 0x3f,  // 1.0f
    0x00, 0x00, 0x00, 0x40,  // 2.0f
    // B: [[5.0, 6.0],[7.0, 8.0]]
    0x00, 0x00, 0xa0, 0x40,  // 5.0f
    0x00, 0x00, 0xc0, 0x40,  // 6.0f
    0x00, 0x00, 0xe0, 0x40,  // 7.0f
    0x00, 0x00, 0x00, 0x41,  // 8.0f
  };
  fuzz_once(seed, sizeof(seed));
  return 0;
}

#endif
