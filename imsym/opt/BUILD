package(default_visibility = ["//visibility:public"])

cc_library(
    name = "opt",
    srcs = [
        "types.hh",
        "key.cc",
        "key.hh",
        "values.cc",
        "values.hh",
        "formatters.hh",
        "values_ops.hh",
        "values_ext_ops.hh",
        "interop.hh",
    ],
    deps = [
        "@automaton_common//motion",
        "@automaton_common//common",
        "@automaton_common//common/formatter",
        "@automaton_common//common/immer",
        "@automaton_common//common/variant",
        "@automaton_common//common:cereal",
        "@symforce_repo//:symforce",
        "@immer",
        "@eigen",
        "@spdlog",
        "@lager",
        "@fmt",
    ],
)

