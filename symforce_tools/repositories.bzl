load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def symforce_deps():
    maybe(
        new_git_repository,
        name = "symforce_repo",
        build_file = "@rules_symforce//symforce_tools:symforce.BUILD",
        remote = "https://github.com/asa/symforce.git",
        commit = "02d9d2a6bd6fcb746ccde6cc790a69021765cee2",
    )
    maybe(
        new_git_repository,
        name = "symengine_repo",
        remote = "https://github.com/asa/symengine.git",
        build_file = "@rules_symforce//symengine_tools:symengine.BUILD",
        commit = "9eb14aadf56639a0b154a44962a888a531fdeb63",
    )
    maybe(
        new_git_repository,
        name = "symenginepy",
        remote = "https://github.com/asa/symenginepy.git",
        commit = "e818d46db53f5482b168e42c0152c3522fdde6a8",
    )
    maybe(
        new_git_repository,
        name = "cython_repo",
        remote = "https://github.com/cython/cython.git",
        tag = "0.29.37.1",
    )
    maybe(
        new_git_repository,
        name = "catch1",
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
        remote = "https://github.com/catchorg/Catch2.git",
        commit = "908c370f245956fd5565207db77641ae1e6d2b36",
    )

def symforce_local_deps(root):
    """assume the repositories are all underneath the same root"""

    native.new_local_repository(
        name = "symforce_repo",
        build_file = "@rules_symforce//symforce_tools:symforce.BUILD",
        path = root + "/symforce",
    )
    native.new_local_repository(
        name = "symengine",
        build_file = "@rules_symforce//symengine_tools:symengine.BUILD",
        path = root + "/symengine",
    )
    native.local_repository(
        name = "cython_repo",
        path = root + "/cython",
    )
    native.local_repository(
        name = "symenginepy",
        path = root + "/symenginepy",
    )
    symforce_deps()
