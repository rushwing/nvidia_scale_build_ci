# ─────────────────────────────────────────────────────────────────────────────
# LEARNING NOTE — linkopts vs deps:
#
#   CMake:  target_link_libraries(hash PRIVATE dl pthread)
#
#   Bazel:  linkopts = ["-ldl", "-lpthread"]
#
#   Why this matters for cross-compilation (Day 6):
#     On the HOST, -ldl resolves to /usr/lib/libdl.so (host glibc).
#     With --config=aarch64, Bazel must find libdl.so inside the SYSROOT,
#     not the host path. If the sysroot is incomplete, you get:
#       "cannot find -ldl"
#     That is the exact class of error you'll debug on Day 6.
# ─────────────────────────────────────────────────────────────────────────────

cc_library(
    name = "hash",
    srcs = ["hash.cc"],
    hdrs = ["hash.h"],
    deps = ["//libs/ffi:clib"],
    # These linkopts propagate up to any cc_binary that transitively deps here.
    linkopts = select({
        "@platforms//os:linux": ["-ldl", "-lpthread"],
        "//conditions:default": [],
    }),
    visibility = ["//visibility:public"],
)
