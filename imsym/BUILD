package(default_visibility = ["//visibility:public"])

# immutable symforce types


cc_library(
    name = "imsym",
    srcs = [
        "imsym.hh",
        "key.cc",
        "key.hh",
        "values.cc",
        "values.hh",
        "formatters.hh",
        "values_ops.hh",
        "values_ext_ops.hh",
    ],
    deps = [
        "@automaton_common//common",
        "@automaton_common//common/immer",
        "@automaton_common//common/variant",
        "@automaton_common//common:cereal",
        "@immer",
        "@symforce",
        "@spdlog",
    ],
)
