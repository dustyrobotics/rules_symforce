load("@symforce_requirements//:requirements.bzl", "requirement")

def _generate_factor_impl(ctx):
    arguments = [
        "generatefactor",
        "--basename",
        ctx.attr.basename,
        "--python_src",
        ctx.file.src.path,
        "--function_name",
        ctx.attr.function_name,# name of python function to codegen from
        "--output_residual_cpp",
        ctx.outputs.output_residual_cpp.path,
    ]
    all_outputs = [ctx.outputs.output_residual_cpp]

    if ctx.attr.function_is_generator:
        arguments.append("--function_is_generator")

    arguments.extend(["--output_factor_cpp", ctx.outputs.output_factor_cpp.path])
    all_outputs.append(ctx.outputs.output_factor_cpp)

    for arg in ctx.attr.arguments:
        arguments.extend(["--arguments", arg])

    for name in ctx.attr.output_names:
        arguments.extend(["--output_names", name])

    ctx.actions.run(
        inputs = [ctx.file.src],
        outputs = all_outputs,
        arguments = arguments,
        mnemonic = "SymforceCompile",
        progress_message = "Compiling symforce...",
        executable = ctx.executable.compiler,
    )

    return struct(
        proto = struct(srcs = [ctx.file.src]),
    )


_generate_factor = rule(
    attrs = {
        "src": attr.label(
            allow_single_file = True,
            mandatory = True,
        ),
        "basename": attr.string(
            mandatory = True,
        ),
        "function_name": attr.string(
            mandatory = True,
        ),
        "function_is_generator": attr.bool(),
        "arguments": attr.string_list(
            mandatory = True,
        ),
        "output_names": attr.string_list(
            mandatory = False,
        ),
        "compiler": attr.label(
            executable = True,
            cfg = "exec",
            default = Label("@rules_symforce//symforce_tools:compiler"),
        ),
        "output_residual_cpp": attr.output(mandatory = True),
        "output_factor_cpp": attr.output(mandatory = False),
    },
    #output_to_genfiles = True,
    implementation = _generate_factor_impl,
)

#the function can be specified via function or function generator
def cc_symforce_factor(name, 
                        src,  # the python source
                        function = None, 
                        function_generator = None, 
                        arguments = [], 
                        output_names = None, 
                        deps = [],
                        tags=[] ):
        
        if (function and function_generator):
            fail("only a function or function_generator must be specified")

        basename = name
        output_residual_cpp = basename + "_residual.h"
        output_factor_cpp = basename + ".h"

        _generate_factor(
            name = name + "_factor_gen",
            basename = name,
            src = src,
            function_name = function_generator or function,
            function_is_generator = True if function_generator != None else False,
            arguments = arguments,
            output_names = output_names,
            output_residual_cpp = output_residual_cpp,
            output_factor_cpp = output_factor_cpp,
        )

        native.py_library(
                name = name + "_pylib",
                srcs = [
                        src,
                ],
                deps = [
                        requirement("symforce-sym"),
                        requirement("symforce"),  # this one is from pip
                        requirement("sympy"),
                        ],
                tags = tags,
                visibility = ["//visibility:public"],
        )

        native.cc_library(
            name = name,
            hdrs = [output_residual_cpp, output_factor_cpp],
            deps = ["@eigen", "@symforce_repo//:symforce"] + deps,
            visibility = ["//visibility:public"],
            tags = tags,
        )


# generate c++ from python using the compiler
# it will read the src file off disk and call symforce.codegen.Codegen.function on it
# with the given args
def _codegen_impl(ctx):
    arguments = [
        "rawcodegen",
        "--basename",
        ctx.attr.basename,
        "--python_src",
        ctx.file.src.path,
        "--function_name",
        ctx.attr.function_name, # name of python function to codegen from
        "--output_cpp",
        ctx.outputs.output_cpp.path,
    ]
    all_outputs = [ctx.outputs.output_cpp]

    for name in ctx.attr.output_names:
        arguments.extend(["--output_names", name])

    if ctx.attr.return_key: 
        arguments.extend(["--return_key", ctx.attr.return_key])


    ctx.actions.run(
        inputs = [ctx.file.src],
        outputs = all_outputs,
        arguments = arguments,
        mnemonic = "SymforceCompile",
        progress_message = "Compiling symforce...",
        executable = ctx.executable.compiler,
    )

    return struct(
        proto = struct(srcs = [ctx.file.src]),
    )


#        cls,
# func: T.Callable,
# config: codegen_config.CodegenConfig,
# name: T.Optional[str] = None,
# input_types: T.Sequence[T.ElementOrType] = None,
# output_names: T.Sequence[str] = None,
# return_key: str = None,
# sparse_matrices: T.Sequence[str] = None,
# docstring: str = None,


_codegen = rule(
    attrs = {
        "src": attr.label(
            allow_single_file = True,
            mandatory = True,
        ),
        "basename": attr.string(
            mandatory = True,
        ),
        "function_name": attr.string(
            mandatory = True,
        ),
        "return_key": attr.string(),
        "output_names": attr.string_list(),
        "compiler": attr.label(
            executable = True,
            cfg = "exec",
            default = Label("@rules_symforce//symforce_tools:compiler"),
        ),
        "output_cpp": attr.output(mandatory = True),
    },
    implementation = _codegen_impl,
)

# generalized codegen for python -> c++
def cc_symforce_library(name, 
                        src, 
                        function = None, 
                        arguments = [], 
                        output_names = None, 
                        return_key = None, 
                        deps = [],
                        tags=[] ):

        basename = name
        output_cpp = basename + ".h"

        _codegen(
            name = name + "_gen",
            basename = basename,
            src = src,
            function_name = function, # what function in src to codegen from
            output_names = output_names,
            return_key = return_key,
            output_cpp = output_cpp,
        )
        native.py_library(
                name = name + "_pylib",
                srcs = [
                        src,
                ],
                deps = [
                        requirement("symforce-sym"),
                        requirement("symforce"),  # this one is from pip
                        requirement("sympy"),
                        ],
                visibility = ["//visibility:public"],
            tags = tags,
        )
        native.cc_library(
            name = name,
            hdrs = [output_cpp],
            deps = ["@eigen", "@symforce_repo//:symforce"] + deps,
            visibility = ["//visibility:public"],
            tags = tags,
        )
