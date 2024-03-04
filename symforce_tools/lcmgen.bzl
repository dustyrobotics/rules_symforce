def _impl(ctx):
    arguments = ["--cpp"]

    # header output dir
    #header_output_path = ctx.genfiles_dir.path + "/" + ctx.attr.src.label.workspace_root + "/lcmtypes"
    header_output_path = ctx.label.name + "_gen" + "/lcmtypes"
    header_output_dir = ctx.actions.declare_directory(header_output_path)
    arguments.extend(["--cpp-hpath", header_output_dir.path])
    arguments.extend(["--cpp-include", "lcmtypes"])

    files = ctx.attr.src[DefaultInfo].files.to_list()
    for file in files:
        print(file.path)
        arguments.append(file.path)# the lcm file

    arguments.extend(["--verbose"])
    arguments.extend(["--print-def"])

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
        #"outs": attr.output_list(),
    },
    output_to_genfiles = True,
    implementation = _impl,
)

def cc_lcm_library(name, 
                   src, 
                   deps = []):
    all_outputs = dict()

    #TODO ideally we parse the input lcm for the stanzas that produce these outputs
    # but we just hardcode these for now as they don't change often
    all_outputs["lcmtypes/symforce.lcm"] = [
        "lcmtypes/sym/index_entry_t.hpp",
        "lcmtypes/sym/index_t.hpp",
        "lcmtypes/sym/key_t.hpp",
        "lcmtypes/sym/linearization_dense_factor_helper_t.hpp",
        "lcmtypes/sym/linearization_dense_key_helper_t.hpp",
        "lcmtypes/sym/linearization_sparse_factor_helper_t.hpp",
        "lcmtypes/sym/linearization_offsets_t.hpp",
        "lcmtypes/sym/linearized_dense_factorf_t.hpp",
        "lcmtypes/sym/linearized_dense_factor_t.hpp",
        "lcmtypes/sym/optimization_iteration_t.hpp",
        "lcmtypes/sym/optimization_stats_t.hpp",
        "lcmtypes/sym/optimization_status_t.hpp",
        "lcmtypes/sym/levenberg_marquardt_solver_failure_reason_t.hpp",
        "lcmtypes/sym/optimizer_gnc_params_t.hpp",
        "lcmtypes/sym/optimizer_params_t.hpp",
        "lcmtypes/sym/imu_biases_t.hpp",
        "lcmtypes/sym/imu_integrated_measurement_delta_t.hpp",
        "lcmtypes/sym/imu_integrated_measurement_derivatives_t.hpp",
        "lcmtypes/sym/imu_integrated_measurement_t.hpp",
        "lcmtypes/sym/sparse_matrix_structure_t.hpp",
        "lcmtypes/sym/valuesf_t.hpp",
        "lcmtypes/sym/values_t.hpp",
    ]
    all_outputs["lcm_types/symforce_types.lcm"] = ["lcm_types/sym/type_t.hpp"]

    _lcmgen(
        name = name,
        src = src,
    )

    #all_hdrs = []

    #for v in all_outputs.values():
    #    all_hdrs.extend(v)

    # make a cc library out of the resulting lcm generated types
    #native.cc_library(
    #    name = name,
    #    hdrs = all_hdrs,
    #    deps = ["@symforce_repo//:skymarshal_core"] + deps,
    #    includes = [".", "lcmtypes"],
    #    visibility = ["//visibility:public"],
    #)
