#include "libs/core/tensor.h"

#include <numeric>
#include <stdexcept>

namespace nvidia::core {

Tensor::Tensor(std::vector<float> d, std::vector<int64_t> s)
    : data(std::move(d)), shape(std::move(s)) {}

int64_t Tensor::num_elements() const {
  if (shape.empty()) return 0;
  return std::accumulate(shape.begin(), shape.end(), int64_t{1},
                         std::multiplies<int64_t>{});
}

bool Tensor::valid() const {
  return static_cast<int64_t>(data.size()) == num_elements();
}

// ── matmul ───────────────────────────────────────────────────────────────────
// Naive O(M*K*N) for correctness; real code would call cuBLAS.
Tensor matmul(const Tensor& a, const Tensor& b) {
  if (a.shape.size() != 2 || b.shape.size() != 2)
    throw std::invalid_argument("matmul: inputs must be 2-D");
  if (a.shape[1] != b.shape[0])
    throw std::invalid_argument("matmul: inner dimensions must match");

  const int64_t M = a.shape[0], K = a.shape[1], N = b.shape[1];
  Tensor out;
  out.shape = {M, N};
  out.data.assign(static_cast<size_t>(M * N), 0.0f);

  for (int64_t i = 0; i < M; ++i)
    for (int64_t k = 0; k < K; ++k)
      for (int64_t j = 0; j < N; ++j)
        out.data[i * N + j] += a.data[i * K + k] * b.data[k * N + j];

  return out;
}

// ── relu ─────────────────────────────────────────────────────────────────────
Tensor relu(const Tensor& x) {
  Tensor out = x;
  for (auto& v : out.data) v = v > 0.0f ? v : 0.0f;
  return out;
}

// ── add ──────────────────────────────────────────────────────────────────────
Tensor add(const Tensor& a, const Tensor& b) {
  if (a.shape != b.shape)
    throw std::invalid_argument("add: shapes must match");
  Tensor out = a;
  for (size_t i = 0; i < out.data.size(); ++i)
    out.data[i] += b.data[i];
  return out;
}

}  // namespace nvidia::core
