# aclint: py2 py3
"Allow the skymarshal package to be executable"
from __future__ import absolute_import
from __future__ import absolute_import

import typing as T

import argparse
import six
import os


from skymarshal.emit_cpp import SkymarshalCpp
from skymarshal.emit_djinni import SkymarshalDjinni
from skymarshal.emit_proto import SkymarshalProto
from skymarshal.emit_proto import SkymarshalProtoLCM
from skymarshal.emit_java import SkymarshalJava
from skymarshal.emit_python import SkymarshalPython
from skymarshal.emit_typescript import SkymarshalTypeScript

from skymarshal.language_plugin import SkymarshalLanguage  # pylint: disable=unused-import
from skymarshal.package_map import parse_lcmtypes


def parse_args(languages, args=None):
    # type: (T.Sequence[T.Type[SkymarshalLanguage]], T.Optional[T.Sequence[str]]) -> argparse.Namespace
    """Parse the argument list and return an options object."""
    parser = argparse.ArgumentParser(description=__doc__, fromfile_prefix_chars="@")
    parser.add_argument("source_path", nargs="+")
    parser.add_argument("--debug-tokens", action="store_true")
    parser.add_argument("--print-def", action="store_true")
    parser.add_argument("--verbose", action="store_true")
    parser.add_argument(
        "--excluded-path",
        nargs="+",
        action="append",
        help="Path to ignore when building package map",
    )

    parser.add_argument(
        "--package-prefix",
        default="",
        help="Add this package name as a prefix to the declared package on types",
    )

    for lang in languages:
        lang.add_args(parser)

    if args:
        # Caller has provided the argument strings explicitly.
        options = parser.parse_args(args)
    else:
        # Use the command-line args.
        options = parser.parse_args()

    return options


def le_main(
    languages,  # type: T.Sequence[T.Type[SkymarshalLanguage]]
    args=None,  # type: T.Sequence[str]
    print_generated=True,  # type: bool
):
    # type: (...) -> None
    """The primary executable for generating lcmtypes code from struct definitions.
    This is mostly an example of how to use the generator."""

    options = parse_args(languages, args)
    package_map = parse_lcmtypes(
        options.source_path,
        verbose=options.verbose,
        print_debug_tokens=options.debug_tokens,
        cache_parser=True,
    )

    packages = list(package_map.values())

    if options.print_def:
        print(packages)

    files = {}

    for lang in languages:
        files.update(lang.create_files(packages, options))

    # Write any generated files that have changed.
    for filename, content in six.iteritems(files):
        dirname = os.path.dirname(filename)
        if bool(dirname) and not os.path.exists(dirname):
            os.makedirs(dirname, 0o755)
        with open(filename, mode="wb") as output_file:
            if isinstance(content, six.text_type):
                good = output_file.write(content.encode("utf-8"))
            else:
                good = output_file.write(content)

    if print_generated:
        print("Generated {} files".format(len(files)))

if __name__ == "__main__":
    le_main(
        [
            SkymarshalCpp,
            SkymarshalDjinni,
            SkymarshalProto,
            SkymarshalProtoLCM,
            SkymarshalJava,
            SkymarshalPython,
            SkymarshalTypeScript,
        ]
    )
