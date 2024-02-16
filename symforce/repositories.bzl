load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def symforce_deps():
    #native.new_local_repository(
    #    name = "symforce",
    #    build_file = "@rules_symforce//symforce:BUILD.symforce",
    #    path = "/home/asa/sbox/symforce"
    #)
    maybe(
        new_git_repository,
        name = "symforce",
        build_file = "@rules_symforce//symforce:BUILD.symforce",
        remote = "https://github.com/asa/symforce.git",
        commit = "7fac7d888b4ec894f7cb66e6d1acc3aa0121d01c",
    )
