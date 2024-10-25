load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")


def symforce_deps():        
    maybe(
        native.new_local_repository,
        name = "gmp",
        build_file = "@rules_symforce//symengine_tools:gmp.BUILD",
        path = "/"
    )
    maybe(
        new_git_repository,
        name = "symforce_repo",
        build_file = "@rules_symforce//symforce_tools:symforce.BUILD",
        remote = "git@github.com:asa/symforce.git",
        commit = "a23ea2c740dcfc751329af59b3d0c5903ade807b"
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
        commit = "762f83db11ff369a0206ac0bc9b5e5b356b434e3",
    )
    maybe(
        new_git_repository,
        name = "cython_repo",
        remote = "https://github.com/asa/cython.git",
        #tag = "0.29.37.1",
        #branch = "autodev",
        commit="34282cc214da10a3d0759067ca1c6ad095051cf1"
    )
    maybe(
        http_archive,
        # this version is claimed by symforce authors to be less buggy
        name = "metis",
        build_file = "@rules_symforce//symforce_tools:metis.BUILD",
        sha256 = "76faebe03f6c963127dbb73c13eab58c9a3faeae48779f049066a21c087c5db2",
        strip_prefix = "metis-5.1.0",
        url = "https://symforce-org.github.io/downloads/metis-5.1.0.tar.gz",
    )
    maybe(
        new_git_repository,
        name = "tl_optional",
        build_file_content = """
cc_library(
    name = "tl_optional",
    srcs = ["include/tl/optional.hpp"],
    includes=["include"],
    visibility = ["//visibility:public"],
)
""",
        commit = "5c4876059c1168d5fa3c945bd8dd05ebbe61b6fe",
        remote = "https://github.com/TartanLlama/optional.git",
        shallow_since = "1561462697 +0100",
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
