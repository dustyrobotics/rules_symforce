package(default_visibility = ["//visibility:public"])

# immutable symforce

cc_library(
    name = "imsym",
    srcs = [
        "imsym.hh",
        "key.cc",
        "key.hh",
        "values.cc",
        "values.hh",
    ],
    deps = [
        "@automaton_common//common",
        "@automaton_common//common/immer",
        "@immer",
        "@symforce",
    ],
)
