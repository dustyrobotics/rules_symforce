package(default_visibility = ["//visibility:public"])

cc_library(
    name = "gklib",
    srcs = glob([
        "GKlib/*.c",
        "GKlib/*.h",
    ]),
    copts=[
        "-Wno-error",
        "-Wno-implicit-function-declaration",
        ],
    includes=["GKlib"],
)

cc_library(
    name = "metis",
    srcs = glob([
        "**/*.c",
        "**/*.h",
    ],
    exclude = [
        "GKlib/**",
        "programs/**",
    ]),
    deps=[
        ":gklib"
    ],
    includes=[
        "include",
        "libmetis",
    ],
    copts=[
        "-Wno-error",
        "-Wno-unused-variable",
        "-Wno-unused-but-set-variable",
        "-Wno-absolute-value",
        ],
)

