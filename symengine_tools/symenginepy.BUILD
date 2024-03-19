package(default_visibility = ["//visibility:public"])
load("@cython//Tools:rules.bzl", "pyx_library")

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



