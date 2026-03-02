#include <cstdint>
#include <iostream>
#include <vector>

#include "api/inference.pb.h"
#include "libs/core/tensor.h"
#include "libs/crypto/hash.h"

// ─────────────────────────────────────────────────────────────────────────────
// inferd: skeleton "inference server" binary.
//
// This binary's sole build-system purpose is to be a cc_binary that:
//   1) Links against //api:inference_cc_proto  (exercises protobuf codegen)
//   2) Links against //libs/core:tensor        (exercises cc_library chain)
//   3) Links against //libs/crypto:hash        (exercises -ldl/-lpthread + ffi)
//   4) Can be cross-compiled to aarch64        (Day 6 cross-toolchain exercise)
//   5) Can be built with --config=hardening    (Day 7 security flags exercise)
//
// It is NOT a real gRPC server — keep it minimal so the build remains fast.
// ─────────────────────────────────────────────────────────────────────────────

namespace {

nvidia::infer::v1::InferResponse run_inference(
    const nvidia::infer::v1::InferRequest& req) {
  // Decode input tensor from proto.
  const auto& proto_input = req.input();
  nvidia::core::Tensor input(
      {proto_input.data().begin(), proto_input.data().end()},
      {proto_input.shape().begin(), proto_input.shape().end()});

  // Stub computation: relu(input) — replace with real model call.
  auto output = nvidia::core::relu(input);

  // Hash the output for content-addressing / trace logging.
  const auto* raw = reinterpret_cast<const uint8_t*>(output.raw_data());
  std::vector<uint8_t> bytes(raw, raw + output.num_elements() * sizeof(float));
  const std::string digest = nvidia::crypto::sha256_hex(bytes);

  nvidia::infer::v1::InferResponse resp;
  for (float v : output.data) resp.mutable_output()->add_data(v);
  for (int64_t s : output.shape) resp.mutable_output()->add_shape(s);
  resp.set_status("ok");
  std::cout << "[inferd] trace=" << req.metadata().count("trace_id")
            << " digest=" << digest << "\n";
  return resp;
}

}  // namespace

int main(int argc, char* argv[]) {
  std::cout << "[inferd] NVIDIA Inference Server skeleton — build system demo\n";

  // Build a synthetic request (simulating what a real client would send).
  nvidia::infer::v1::InferRequest req;
  req.set_model_id("resnet50-stub");
  (*req.mutable_metadata())["trace_id"] = "demo-0001";

  // 2×2 input tensor [1,2,3,4].
  for (float v : {1.0f, 2.0f, 3.0f, 4.0f}) req.mutable_input()->add_data(v);
  req.mutable_input()->add_shape(2);
  req.mutable_input()->add_shape(2);

  const auto resp = run_inference(req);
  std::cout << "[inferd] status=" << resp.status()
            << " latency=" << resp.latency_us() << "us\n";
  std::cout << "[inferd] ready — press Ctrl-C to stop\n";
  return 0;
}
