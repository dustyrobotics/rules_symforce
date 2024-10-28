package(default_visibility = ["//visibility:public"])

#gmp needs to be installed on the system
cc_library(
    name = "gmp",
    hdrs = select({
        "@platforms//os:linux": ["usr/include/x86_64-linux-gnu/gmp.h"],
        "@platforms//os:macos": ["opt/homebrew/include/gmp.h"],
        "//conditions:default": [],
    }),
    includes = select({
        "@platforms//os:linux": ["usr/include"],
        "@platforms//os:macos": ["opt/homebrew/include"],
        "//conditions:default": [],
    }),
    linkopts = select({
        "@platforms//os:linux": ["-lgmp"],
        "@platforms//os:macos": [
            "-L/opt/homebrew/lib",
            "-lgmp",
        ],
        "//conditions:default": [],
    }),
)
