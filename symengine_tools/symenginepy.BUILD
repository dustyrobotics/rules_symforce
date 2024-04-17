package(default_visibility = ["//visibility:public"])
load("@cython//Tools:rules.bzl", "pyx_library")

load("@rules_symforce//symengine_tools:config.bzl", "symenginepy_config")

symenginepy_config(
     name="config",
     template = "symengine/lib/config.pxi.in",
     output = "symengine/lib/config.pxi",
)

cc_library(
        name = "pywrapper",
        hdrs = ["symengine/lib/pywrapper.h"],
        srcs = ["symengine/lib/pywrapper.cpp"],
        deps = [
            "@symengine_repo//:symengine",
            "@python",
#            "@cython//cython",
        ],
)

#config_pxi ="""\
#DEF HAVE_SYMENGINE_MPFR = False\
#DEF HAVE_SYMENGINE_MPC = False\
#DEF HAVE_SYMENGINE_PIRANHA = False\
#DEF HAVE_SYMENGINE_FLINT = False\
#DEF HAVE_SYMENGINE_LLVM = False\
#DEF HAVE_SYMENGINE_LLVM_LONG_DOUBLE = False
#"""

pyx_library(
    name = "symengine_wrapper",
    srcs = [
            "symengine/lib/symengine_wrapper.pxd",
            "symengine/lib/config.pxi",
    ],
)

pyx_library(
    name = "symengine",
    srcs = [
        "symengine/lib/symengine.pxd",
        "symengine/lib/config.pxi",
    ],
)

py_library(
        name = "lib",
        srcs = [
                "symengine/lib/__init__.py",
        ],
        deps = [
                ":symengine_wrapper",
                ":symengine",
        ],
        data = [
                ":pywrapper"
        ],
)

py_library(
        name = "symenginepy",
        srcs = [
                "symengine/__init__.py",
                "symengine/printing.py",
                "symengine/functions.py",
                "symengine/utilities.py",
                "symengine/sympy_compat.py",
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
