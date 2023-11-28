# rules_symforce

Bazel rules for [symforce](https://symforce.org/)

## Installing

Add the following lines to your WORKSPACE file to download and initialize rules_symforce in your repo:

```bzl
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "rules_symforce",
    sha256 = "c75b4ebcd4f7db7ac2a2a6a5be3c64599c97a26934db67de51d3935304ab6e8c",
    strip_prefix = "rules_symforce-0.1.0",
    urls = ["https://github.com/asa/rules_symforce/archive/refs/tags/v0.1.0.tar.gz"],
)

load(
    "@rules_symforce//symforce:repositories.bzl",
    "symforce_dependencies",
)

symforce_deps()
```

