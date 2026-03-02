# Package marker — makes build_defs/ a Bazel package.
# Starlark macros here are loaded by other BUILD files with:
#   load("//build_defs:secure_cc.bzl", "hardened_cc_binary")
exports_files(
    glob(["*.bzl"]),
    visibility = ["//visibility:public"],
)
