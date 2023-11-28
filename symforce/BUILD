load("codegen.bzl", "cc_symforce_library")
load("lcmgen.bzl", "cc_lcm_library")
load("@symforce_requirements//:requirements.bzl", "requirement")

py_binary(
    name = "compiler",
    srcs = ["compiler.py"],
    main = "compiler.py",
    visibility = ["//visibility:public"],
    deps = [
        requirement("click"),
        requirement("symforce-sym"),
        requirement("symforce"),  # this one is from pip
        requirement("sympy"),
    ],
)

py_binary(
    name = "lcmgen",
    srcs = ["lcmgen.py"],
    main = "lcmgen.py",
    visibility = ["//visibility:public"],
    deps = [
        requirement("skymarshal"),
        requirement("typing-extensions"),
    ],
)
