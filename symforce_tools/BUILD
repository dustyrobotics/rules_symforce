load("codegen.bzl", "cc_symforce_library")
load("lcmgen.bzl", "cc_lcm_library")
load("@symforce_requirements//:requirements.bzl", "requirement")

package(default_visibility = ["//visibility:public"])

py_binary(
    name = "compiler",
    srcs = ["compiler.py"],
    main = "compiler.py",
    deps = [
        requirement("click"),
        requirement("symforce-sym"),  # the generated symforce python package
        "@symforce_repo//:py",
        requirement("sympy"),
    ],
)

py_binary(
    name = "lcmgen",
    srcs = ["lcmgen.py"],
    main = "lcmgen.py",
    deps = [
        requirement("skymarshal"),
        requirement("typing-extensions"),
    ],
)

py_library(
    name = "backend_configs",
    srcs = glob([
        "codegen/backends/*/*.py",
    ]),
    deps = [
    ],
    data = [
            ":backends"
    ],
)

filegroup(
    name = "backends",
    srcs = glob([
        "codegen/backends/**"
    ]),
)

# stripped versions of the codegen files in symforce source
# they are stripped so that lcm types can be injected and agregated through bazel
# less "magic" and more explicit
py_library(
    name = "codegen_stripped",
    srcs = [
        "codegen/geo_package_codegen_stripped.py",
        "codegen/sym_util_package_codegen_stripped.py",
    ],
    deps = [
        "@symforce_repo//:py",
    ],
    data = [
            ":backends"
    ],
)

# generator to run stripped versions of the symforce codegen
# using injected templates and user types
py_binary(
    name = "generate_gen",
    srcs = ["generate_gen.py"],
    main = "generate_gen.py",
    deps = [
        "@symforce_repo//:py",
        ":codegen_stripped",
        ":backend_configs",
    ],
)
