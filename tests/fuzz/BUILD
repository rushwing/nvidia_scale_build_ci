# ─────────────────────────────────────────────────────────────────────────────
# LEARNING NOTE — fuzz targets in Bazel:
#
#   Real libFuzzer workflow:
#     bazel build //tests/fuzz:fuzz_core \
#       --config=asan \
#       --copt=-fsanitize=fuzzer \
#       --linkopt=-fsanitize=fuzzer
#
#   We keep it as cc_test with a standalone main so `bazel test //...` works
#   in CI without a fuzzer-enabled compiler.
#   On Day 7, switch to cc_binary + fuzzer sanitizer config.
# ─────────────────────────────────────────────────────────────────────────────

cc_test(
    name = "fuzz_core",
    size = "small",
    srcs = ["fuzz_core.cc"],
    deps = ["//libs/core:tensor"],
    # 'manual': don't include in //... expansion; run explicitly.
    # Remove this tag when you want CI to always run the fuzz smoke-test.
    tags = ["manual"],
)
