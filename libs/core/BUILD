# ─────────────────────────────────────────────────────────────────────────────
# LEARNING NOTE (CMake → Bazel):
#
#   CMake:  add_library(tensor tensor.cc)
#           target_include_directories(tensor PUBLIC ${CMAKE_SOURCE_DIR})
#
#   Bazel:  cc_library(name="tensor", srcs=[...], hdrs=[...])
#
#   Key difference in `hdrs` vs `srcs`:
#     hdrs = headers that other targets can #include  (exported)
#     srcs = files compiled only for THIS target     (private)
#
#   Bazel enforces this: if a dep tries to include a file not in hdrs,
#   the build fails (hermetic header check). CMake has no such enforcement.
# ─────────────────────────────────────────────────────────────────────────────

cc_library(
    name = "tensor",
    srcs = ["tensor.cc"],
    hdrs = ["tensor.h"],
    # No external deps: this is the "leaf" of the C++ dep graph.
    # services/infer and tests/fuzz both depend on this target.
    visibility = ["//visibility:public"],
)
