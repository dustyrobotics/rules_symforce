load("codegen.bzl", "cc_symforce_library")
load("lcmgen.bzl", "cc_lcm_library")
load("@symforce_requirements//:requirements.bzl", "requirement")

package(default_visibility = ["//visibility:public"])

# generator to run stripped versions of the symforce codegen
# using injected templates and user types
py_binary(
    name = "generate_gen_no_sym",
    srcs = ["generate_gen_no_sym.py"],
    main = "generate_gen_no_sym.py",
    args = ["--color", "always"],
    deps = [
        ":codegen_utils",
        requirement("click"),
        requirement("ruff"),
    ],
)

py_binary(
    name = "generate_gen",
    srcs = ["generate_gen.py"],
    main = "generate_gen.py",
    deps = [
        ":codegen_utils",
        requirement("click"),
        requirement("ruff"),
        "@symforce_repo//:symforce_sym",
    ],
)

# compiler is all about making factors
py_binary(
    name = "compiler",
    srcs = ["compiler.py"],
    main = "compiler.py",
    deps = [
        ":codegen_utils",
        "@symforce_repo//:symforce_sym",
        requirement("click"),
    ],
    data = [
        # process won't recognize these as python files, load as data
        "@symforce_repo//:symforce_sym",
    ],
)

py_binary(
    name = "lcmgen",
    srcs = ["lcmgen.py"],
    main = "lcmgen.py",
    deps = [
        "@symforce_repo//:skymarshal",
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

filegroup(
    name = "lcm_templates",
    srcs = glob([
        "codegen/lcm_templates/**"
    ]),
)

# stripped versions of the codegen files in symforce source
# they are stripped down so that lcm types can be injected and agregated through bazel
# less "magic" and more explicit
py_library(
    name = "codegen_utils",
    srcs = [
        "codegen/geo_package_codegen_stripped.py",
        "codegen/cam_package_codegen_stripped.py",
        "codegen/geo_factors_codegen_stripped.py",
        "codegen/slam_factors_codegen_stripped.py",
        "codegen/types_package_codegen_stripped.py",
        "codegen/sym_util_package_codegen_stripped.py",
        "codegen/lcm_template_dir.py",
    ],
    deps = [
        "@symforce_repo//:py",
        ":backend_configs",
        requirement("clang-format"),
        requirement("typing-extensions"),
        requirement("jinja2"),
    ],
    data = [
        ":lcm_templates"
    ],
)

