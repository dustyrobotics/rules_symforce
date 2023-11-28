cc_test(
    name = "test",
    srcs = [
        "test.cc",
    ],
    copts = ["-Wno-unused-variable"],
    linkstatic = True,
    deps = [
        "//common/imsym",
        "//solvers/lee/factors",
        "@catch2//:catch2_main",
    ],
)
