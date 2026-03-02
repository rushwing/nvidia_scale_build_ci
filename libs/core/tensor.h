#pragma once

#include <cstdint>
#include <stdexcept>
#include <vector>

namespace nvidia::core {

// ─────────────────────────────────────────────────────────────────────────────
// Tensor: minimal dense tensor (stub for build-system demo purposes).
//
// Real NVIDIA code would use cuTensor / TensorRT types; this stub exists to:
//   1) Create a cc_library that services/infer and tests/fuzz both depend on.
//   2) Give Sanitizers / fuzzer something meaningful to exercise.
//   3) Produce symbols whose cross-compile correctness we can verify.
// ─────────────────────────────────────────────────────────────────────────────

struct Tensor {
  std::vector<float>   data;
  std::vector<int64_t> shape;

  Tensor() = default;
  Tensor(std::vector<float> data, std::vector<int64_t> shape);

  float*       mutable_data()        { return data.data(); }
  const float* raw_data()      const { return data.data(); }

  int64_t num_elements() const;
  bool    valid()        const;  // shape consistent with data size
};

// ── Ops (all throw on bad shapes) ───────────────────────────────────────────

Tensor matmul(const Tensor& a, const Tensor& b);
Tensor relu(const Tensor& x);
Tensor add(const Tensor& a, const Tensor& b);

}  // namespace nvidia::core
