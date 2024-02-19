package(default_visibility = ["//visibility:public"])

cc_library(
    name = "conversions",
    srcs = [
        "conversions.hh",
    ],
    deps = [
        ":factor_graph",
        "//imsym",
        "@eigen",
    ],
)


cc_library(
    name = "factor_graph",
    srcs = [
        "types.hh",
    ],
    deps = [
        "@automaton_common//common",
        "@automaton_common//motion",
        "@automaton_common//common:cereal",
        "//imsym",
        "@lager",
    ],
)


