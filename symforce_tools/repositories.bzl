load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def symforce_deps():
    native.new_local_repository(
        name = "symforce_repo",
        build_file = "@rules_symforce//symforce_tools:symforce.BUILD",
        path = "/home/asa/sbox/symforce"
    )
    native.new_local_repository(
        name = "symengine_repo",
        build_file = "@rules_symforce//symengine_tools:symengine.BUILD",
        path = "/home/asa/sbox/symforce/third_party/symengine"
    )
    native.local_repository(
        name = "symenginepy",
        #build_file = "@rules_symforce//symengine_tools:symenginepy.BUILD",
        path = "/home/asa/sbox/symforce/third_party/symenginepy"
    )
    new_git_repository(
        name = "cython",
        remote = "https://github.com/cython/cython.git",
        tag = "0.29.37.1",

    )
    native.new_local_repository(
        name = "catch1",
        #build_file = "@rules_symforce//symengine_tools:catch.BUILD",
        build_file_content = """cc_library(
    name = "catch1",
    hdrs = glob([
        "include/catch.hpp",
        "include/**/*.h*",
    ]),
    srcs = [],
    includes = ["include"],
    copts = [
        ],
    visibility = ["//visibility:public"],
)""",
        path = "/home/asa/sbox/Catch2"
    )
    #new_git_repository(
    #    name = "catch2_old",
    #    remote = "https://github.com/catchorg/Catch2.git",
    #     commit="908c370f245956fd5565207db77641ae1e6d2b36",
    #)
    #maybe(
    #    new_git_repository,
    #    name = "symforce",
    #    build_file = "@rules_symforce//symforce_tools:BUILD.symforce",
    #    remote = "https://github.com/asa/symforce.git",
    #    commit = "7fac7d888b4ec894f7cb66e6d1acc3aa0121d01c",
    #)
