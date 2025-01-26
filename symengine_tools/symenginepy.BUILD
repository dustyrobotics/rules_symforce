load("@cython_repo//Tools:rules.bzl", "pyx_library")
load("@rules_symforce//symengine_tools:config.bzl", "symenginepy_config")

package(default_visibility = ["//visibility:public"])

symenginepy_config(
    name = "config",
    output = "symengine/lib/config.pxi",
    template = "symengine/lib/config.pxi.in",
)

cc_library(
    name = "pywrapper",
    srcs = ["symengine/lib/pywrapper.cpp"],
    hdrs = ["symengine/lib/pywrapper.h"],
    deps = [
        "@python_dev",
        "@symengine_repo//:symengine",
    ],
)

pyx_library(
    name = "symengine_wrapper",
    srcs = [
        "symengine/lib/config.pxi",
        "symengine/lib/symengine_wrapper.pxd",
    ],
)

pyx_library(
    name = "symengine",
    srcs = [
        "symengine/lib/config.pxi",
        "symengine/lib/symengine.pxd",
    ],
)

py_library(
    name = "lib",
    srcs = [
        "symengine/lib/__init__.py",
    ],
    data = [
        ":pywrapper",
    ],
    deps = [
        ":symengine",
        ":symengine_wrapper",
    ],
)

py_library(
    name = "symenginepy",
    srcs = [
        "symengine/__init__.py",
        "symengine/functions.py",
        "symengine/printing.py",
        "symengine/sympy_compat.py",
        "symengine/utilities.py",
    ],
    deps = [
        ":lib",
    ],
)

py_test(
    name = "test_arit",
    srcs = glob(["symengine/tests/test_arit.py"]),
    deps = [
        ":symenginepy",
    ],
)
