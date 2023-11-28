def _impl(ctx):
    arguments = [
        "compile",
        "--input_path",
        ctx.file.src.path,
        "--function",
        ctx.attr.function,
        "--output_residual",
        ctx.outputs.out_residual.path,
    ]
    all_outputs = [ctx.outputs.out_residual]

    if ctx.attr.should_output_factor:
        arguments.extend(["--output_factor", ctx.outputs.out_factor.path])
        all_outputs.append(ctx.outputs.out_factor)

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

_symforce = rule(
    attrs = {
        "src": attr.label(
            allow_single_file = True,
            mandatory = True,
        ),
        "function": attr.string(
            mandatory = True,
        ),
        "arguments": attr.string_list(
            mandatory = True,
        ),
        "output_names": attr.string_list(
            mandatory = True,
        ),
        "compiler": attr.label(
            executable = True,
            cfg = "exec",
            default = Label("@rules_symforce//symforce:compiler"),
        ),
        "should_output_factor": attr.bool(),
        "out_residual": attr.output(mandatory = True),
        "out_factor": attr.output(mandatory = False),
    },
    #output_to_genfiles = True,
    implementation = _impl,
)

def cc_symforce_library(name, src, function, arguments = [], should_output_factor = True, output_names = [], deps = []):
    if should_output_factor:
        _symforce(
            name = name + "_factor_gen",
            src = src,
            function = function,
            arguments = arguments,
            output_names = output_names,
            out_residual = name + "_residual.h",
            should_output_factor = should_output_factor,
            out_factor = name + "_factor.h",
        )

        native.cc_library(
            name = name,
            hdrs = [name + "_residual.h", name + "_factor.h"],
            deps = ["@eigen", "@symforce"] + deps,
            visibility = ["//visibility:public"],
        )
    else:
        # for imu factor for instance
        _symforce(
            name = name + "_factor_gen",
            src = src,
            function = function,
            arguments = arguments,
            output_names = output_names,
            out_residual = name + "_residual.h",
            should_output_factor = False,
        )

        native.cc_library(
            name = name,
            hdrs = [name + "_residual.h"],
            deps = ["@eigen", "@symforce"] + deps,
            visibility = ["//visibility:public"],
        )
