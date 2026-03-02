# build_defs/secure_cc.bzl
#
# Macros that wrap cc_binary / cc_library with NVIDIA-style hardening flags.
#
# LEARNING NOTE:
#   In CMake you'd add_compile_options() globally or per-target — but it's
#   easy to forget, and there's no enforcement.
#
#   In Bazel, you wrap the rule in a macro. Every target using hardened_cc_binary
#   gets the flags by construction. You can audit with:
#     bazel cquery 'attr(name, "inferd", //...)'
#     bazel aquery 'mnemonic("CppCompile", //services/infer:inferd)' \
#       --config=hardening
#   and see every -fstack-protector-strong in the action command line.

# ── Linux hardening flags ─────────────────────────────────────────────────────
_LINUX_HARDENING_COPTS = [
    "-fstack-protector-strong",   # stack canaries on functions with buffers
    "-D_FORTIFY_SOURCE=2",        # glibc buffer-overflow checks at runtime
    "-fPIE",                      # position-independent executable (for ASLR)
    "-Wformat",                   # format-string warnings
    "-Wformat-security",
    "-Werror=format-security",
]

_LINUX_HARDENING_LINKOPTS = [
    "-pie",              # enable ASLR in the loader
    "-Wl,-z,relro",     # make GOT read-only after startup (partial RELRO)
    "-Wl,-z,now",       # resolve all symbols at load time (full RELRO)
    "-Wl,-z,noexecstack",
]

def hardened_cc_binary(name, copts = [], linkopts = [], **kwargs):
    """Drop-in replacement for cc_binary that adds security hardening flags."""
    native.cc_binary(
        name = name,
        copts = copts + select({
            "@platforms//os:linux": _LINUX_HARDENING_COPTS,
            "//conditions:default": [],
        }),
        linkopts = linkopts + select({
            "@platforms//os:linux": _LINUX_HARDENING_LINKOPTS,
            "//conditions:default": [],
        }),
        **kwargs
    )

def hardened_cc_library(name, copts = [], **kwargs):
    """Drop-in replacement for cc_library that adds security hardening flags."""
    native.cc_library(
        name = name,
        copts = copts + select({
            "@platforms//os:linux": _LINUX_HARDENING_COPTS,
            "//conditions:default": [],
        }),
        **kwargs
    )
