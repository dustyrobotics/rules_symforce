cc_test(
    name = "test",
    srcs = [
        "test.cc",
    ],
    copts = ["-Wno-unused-variable"],
    linkstatic = True,
    deps = [
        "//imsym",
        "@spdlog",
        "@catch2//:catch2_main",
    ],
)
