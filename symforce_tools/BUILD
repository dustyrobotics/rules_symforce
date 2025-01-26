load("@symforce_requirements//:requirements.bzl", "requirement")
load("codegen.bzl", "cc_symforce_library")
load("lcmgen.bzl", "cc_lcm_library")

package(default_visibility = ["//visibility:public"])

# generator to run stripped versions of the symforce codegen
# using injected templates and user types
py_binary(
    name = "generate_gen_no_sym",
    srcs = ["generate_gen_no_sym.py"],
    args = [
        "--color",
        "always",
    ],
    main = "generate_gen_no_sym.py",
    deps = [
        ":codegen_utils",
        requirement("click"),
    ],
)

py_binary(
    name = "generate_gen",
    srcs = ["generate_gen.py"],
    main = "generate_gen.py",
    deps = [
        ":codegen_utils",
        requirement("click"),
        "@symforce_repo//:symforce_sym",
    ],
)

# compiler is all about making factors
py_binary(
    name = "compiler",
    srcs = ["compiler.py"],
    data = [
        # process won't recognize these as python files, load as data
        # TODO change this to call these files python files
        "@symforce_repo//:symforce_sym",
    ],
    main = "compiler.py",
    deps = [
        ":codegen_utils",
        "@symforce_repo//:symforce_sym",
        requirement("click"),
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
    data = [
        ":backends",
    ],
    deps = [
    ],
)

filegroup(
    name = "backends",
    srcs = glob([
        "codegen/backends/**",
    ]),
)

filegroup(
    name = "lcm_templates",
    srcs = glob([
        "codegen/lcm_templates/**",
    ]),
)

# stripped versions of the codegen files in symforce source
# they are stripped down so that lcm types can be injected and agregated through bazel
# less "magic" and more explicit
py_library(
    name = "codegen_utils",
    srcs = [
        "codegen/cam_package_codegen_stripped.py",
        "codegen/geo_factors_codegen_stripped.py",
        "codegen/geo_package_codegen_stripped.py",
        "codegen/lcm_template_dir.py",
        "codegen/slam_factors_codegen_stripped.py",
        "codegen/sym_util_package_codegen_stripped.py",
        "codegen/types_package_codegen_stripped.py",
    ],
    data = [
        ":lcm_templates",
    ],
    deps = [
        ":backend_configs",
        "@symforce_repo//:py",
        requirement("clang-format"),
        requirement("typing-extensions"),
        requirement("jinja2"),
    ],
)
