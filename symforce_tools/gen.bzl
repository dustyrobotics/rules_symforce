# an imple that mirrors the interface of the generate_gen python tool

def _generate_gen_impl(ctx):
    subcommand = ctx.label.name.replace("_", "-")
    output_dir = ctx.actions.declare_directory(ctx.label.name + "_gen")
    arguments = [subcommand, "--output_dir", output_dir.path]

    for t in ctx.attr.geo_types:
        arguments.append("--geo_types")
        arguments.append(t)

    for t in ctx.attr.cam_types:
        arguments.append("--cam_types")
        arguments.append(t)


    ctx.actions.run(
        inputs = [], # backends
        outputs = [output_dir],
        arguments = arguments,
        mnemonic = "SymforceCompile",
        progress_message = "Compiling symforce...",
        executable = ctx.executable.compiler,
    )

    # ideally we split this into headers and cc
    compilation_context = cc_common.create_compilation_context(
        headers = depset([output_dir]),
        includes = depset([output_dir.path + "/cpp"]),
    )
    return [
            DefaultInfo(files = depset([output_dir])),
            CcInfo(
                compilation_context = compilation_context,
            )
    ]

def _generate_py_gen_impl(ctx):
    subcommand = ctx.label.name.replace("_", "-")
    output_dir = ctx.actions.declare_directory(ctx.label.name + "_gen")
    arguments = [subcommand, "--output_dir", output_dir.path]

    for t in ctx.attr.geo_types:
        arguments.append("--geo_types")
        arguments.append(t)

    for t in ctx.attr.cam_types:
        arguments.append("--cam_types")
        arguments.append(t)


    ctx.actions.run(
        inputs = [], # backends
        outputs = [output_dir],
        arguments = arguments,
        mnemonic = "SymforceCompile",
        progress_message = "Generating symforce package...",
        executable = ctx.executable.compiler,
    )

    return [
            DefaultInfo(files = depset([output_dir])),
            PyInfo(
                    transitive_sources = depset([output_dir])
            )
    ]

def _generate_gen_lcm_impl(ctx):
    subcommand = ctx.label.name.replace("_", "-")
    output_lcm_file = ctx.actions.declare_file(ctx.label.name + ".lcm")
    arguments = [subcommand, "--output_lcm", output_lcm_file.path]

    for t in ctx.attr.geo_types:
        arguments.append("--geo_types")
        arguments.append(t)

    for t in ctx.attr.cam_types:
        arguments.append("--cam_types")
        arguments.append(t)

    ctx.actions.run(
        inputs = [],
        outputs = [output_lcm_file],
        arguments = arguments,
        mnemonic = "SymforceCompileLcm",
        progress_message = "Compiling symforce into lcm types...",
        executable = ctx.executable.compiler,
    )

    # lcm provider?
    return [
            DefaultInfo(files = depset([output_lcm_file])),
    ]

_gen_sym_geo_resource = rule(
    attrs = {
        "geo_types": attr.string_list(
                mandatory = True,
                doc = "The types to generate c++ for",
        ),
        "compiler": attr.label(
            executable = True,
            cfg = "exec",
            default = Label("@rules_symforce//symforce_tools:generate_gen"),
        ),
    },
    implementation = _generate_gen_impl,
)

_gen_sym_package = rule(
    attrs = {
        "geo_types": attr.string_list(
                doc = "The geo types to generate c++ for",
        ),        
        "cam_types": attr.string_list(
                doc = "The camera types to generate c++ for",
        ),
        "compiler": attr.label(
            executable = True,
            cfg = "exec",
            default = Label("@rules_symforce//symforce_tools:generate_gen"),
        ),
    },
    implementation = _generate_gen_impl,
)

_gen_py_sym_package = rule(
    attrs = {
        "geo_types": attr.string_list(
                doc = "The types to generate c++ for",
        ),        
        "cam_types": attr.string_list(
                doc = "The types to generate c++ for",
        ),
        "compiler": attr.label(
            executable = True,
            cfg = "exec",
            default = Label("@rules_symforce//symforce_tools:generate_gen"),
        ),
    },
    implementation = _generate_py_gen_impl,
)

_gen_sym_util_package = rule(
    attrs = {        
        "geo_types": attr.string_list(
                mandatory = True,
                doc = "The geo types to generate c++ for",
        ),        
        "cam_types": attr.string_list(
                mandatory = True,
                doc = "The camera types to generate c++ for",
        ),
        "compiler": attr.label(
            executable = True,
            cfg = "exec",
            default = Label("@rules_symforce//symforce_tools:generate_gen"),
        ),
    },
    implementation = _generate_gen_impl,
)

_gen_sym_lcm_package = rule(
    attrs = {        
        "geo_types": attr.string_list(
                mandatory = True,
                doc = "The geo types to generate c++ for",
        ),        
        "cam_types": attr.string_list(
                mandatory = True,
                doc = "The camera types to generate c++ for",
        ),
        "compiler": attr.label(
            executable = True,
            cfg = "exec",
            default = Label("@rules_symforce//symforce_tools:generate_gen"),
        ),
    },
    implementation = _generate_gen_lcm_impl,
)

# generate a typed resource
def cc_gen_pkg(name,
               geo_types = ["sf.Rot2", "sf.Rot3", "sf.Pose2", "sf.Pose3", "sf.Unit3"],
               cam_types = []
        ):
        _gen_sym_package(
                name = name,
                geo_types = geo_types,
                cam_types = cam_types,        
        )

def py_gen_pkg(name,
               geo_types = ["sf.Rot2", "sf.Rot3", "sf.Pose2", "sf.Pose3", "sf.Unit3"],
               cam_types = []
        ):
        _gen_py_sym_package(
                name = name,
                geo_types = geo_types,
                cam_types = cam_types,        
        )


def lcm_pkg(name,
               geo_types = ["sf.Rot2", "sf.Rot3", "sf.Pose2", "sf.Pose3", "sf.Unit3"],
               cam_types = []
        ):
        _gen_sym_lcm_package(
                name = name,
                geo_types = geo_types,
                cam_types = cam_types,        
        )


def cc_sym_util_pkg(name = "sym_util_package",
               geo_types = ["sf.Rot2", "sf.Rot3", "sf.Pose2", "sf.Pose3", "sf.Unit3"],
               cam_types = []
        ):
        _gen_sym_util_package(
                name = name,
                geo_types = geo_types,
                cam_types = cam_types,
        )
