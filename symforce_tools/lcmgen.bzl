def _impl(ctx):
    arguments = ["--cpp"]

    # header output dir
    header_output_path = ctx.label.name + "_gen" + "/lcmtypes"
    header_output_dir = ctx.actions.declare_directory(header_output_path)
    arguments.extend(["--cpp-hpath", header_output_dir.path])
    arguments.extend(["--cpp-include", "lcmtypes"])

    files = ctx.attr.src[DefaultInfo].files.to_list()
    for file in files:
        #print(file.path)
        arguments.append(file.path)# the lcm file

    #arguments.extend(["--verbose"])
    #arguments.extend(["--print-def"])

    ctx.actions.run(
        inputs = [ctx.file.src],
        outputs = [header_output_dir],
        mnemonic = "LcmCompile",
        progress_message = "generating lcm types...",
        arguments = arguments,
        executable = ctx.executable._compiler,
    )

    # ideally we split this into headers and cc
    compilation_context = cc_common.create_compilation_context(
        headers = depset([header_output_dir]),
        includes = depset([header_output_dir.path,
                           header_output_dir.path.replace("/lcmtypes", "")]),
    )
    return [
            DefaultInfo(files = depset([header_output_dir])),
            CcInfo(
                compilation_context = compilation_context,
            )
    ]

_lcmgen = rule(
    attrs = {
        "src": attr.label(
            allow_single_file = True,
            mandatory = True,
        ),
        "_compiler": attr.label(
            executable = True,
            cfg = "exec",
            default = Label("@rules_symforce//symforce_tools:lcmgen"),
        ),
    },
    implementation = _impl,
)

def cc_lcm_library(name, 
                   src, 
                   deps = []):
    _lcmgen(
        name = name,
        src = src,
    )
