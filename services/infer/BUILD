# ─────────────────────────────────────────────────────────────────────────────
# LEARNING NOTE — cc_binary vs cc_library:
#
#   CMake:  add_executable(inferd main.cc)
#           target_link_libraries(inferd PRIVATE tensor hash inference_proto)
#
#   Bazel:  cc_binary(name="inferd", srcs=["main.cc"], deps=[...])
#
#   The `deps` list here IS the full dependency declaration.
#   Bazel derives the link order, include paths, and linkopts transitively.
#   You NEVER manually specify -I flags or library search paths.
#
# WHAT THIS TRIGGERS (aquery will show all of these as Actions):
#   GenProto     → inference.pb.cc / inference.pb.h
#   CppCompile   → clib.c.o, tensor.cc.o, hash.cc.o, main.cc.o
#   CppLink      → inferd  (with -ldl -lpthread from //libs/crypto:hash)
# ─────────────────────────────────────────────────────────────────────────────

cc_binary(
    name = "inferd",
    srcs = ["main.cc"],
    deps = [
        "//api:inference_cc_proto",
        "//libs/core:tensor",
        "//libs/crypto:hash",
    ],
)
