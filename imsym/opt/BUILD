package(default_visibility = ["//visibility:public"])

cc_library(
    name = "opt",
    srcs = [
        "formatters.hh",
        "interop.hh",
        "key.cc",
        "key.hh",
        "types.hh",
        "values.cc",
        "values.hh",
        "values_ext_ops.hh",
        "values_ops.hh",
    ],
    deps = [
        "@automaton_common//common",
        "@automaton_common//common:cereal",
        "@automaton_common//common/formatter",
        "@automaton_common//common/hash",
        "@automaton_common//common/immer",
        "@automaton_common//common/variant",
        "@automaton_common//motion",
        "@eigen",
        "@fmt",
        "@immer",
        "@lager",
        "@spdlog",
        "@symforce_repo//:symforce",
    ],
)
