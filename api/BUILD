load("@rules_proto//proto:defs.bzl", "proto_library")

# ─────────────────────────────────────────────────────────────────────────────
# LEARNING NOTE (CMake → Bazel):
#
#   CMake:  find_package(Protobuf REQUIRED)
#           protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS inference.proto)
#           add_library(api ${PROTO_SRCS} ${PROTO_HDRS})
#
#   Bazel:  proto_library        → declares the .proto file as a "proto target"
#           cc_proto_library     → derives a C++ library from the proto target
#
#   Key difference: Bazel knows the exact inputs/outputs of code generation.
#   It runs protoc as a hermetic Action, hashes its inputs, caches the output.
#   No "clean && cmake .." surprises.
# ─────────────────────────────────────────────────────────────────────────────

proto_library(
    name = "inference_proto",
    srcs = ["inference.proto"],
    visibility = ["//visibility:public"],
)

# cc_proto_library is a native Bazel rule (no explicit load needed in Bazel 7).
# It runs: protoc --cpp_out=... inference.proto
# and wraps the generated .pb.cc / .pb.h in a cc_library automatically.
cc_proto_library(
    name = "inference_cc_proto",
    deps = [":inference_proto"],
    visibility = ["//visibility:public"],
)
