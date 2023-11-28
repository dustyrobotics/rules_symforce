def _impl(ctx):
    cmd = []
    cmd.extend(["--cpp"])

    # header output dir
    header_output_dir = ctx.genfiles_dir.path + "/" + ctx.attr.src.label.workspace_root + "/lcmtypes"
    cmd.extend(["--cpp-hpath", header_output_dir])
    cmd.extend(["--cpp-include", "lcmtypes"])

    cmd.append(ctx.file.src.path)

    cmd.extend(["--verbose"])
    cmd.extend(["--print-def"])

    ctx.actions.run(
        inputs = [ctx.file.src],
        outputs = ctx.outputs.outs,
        mnemonic = "LcmCompile",
        progress_message = "generating lcm types...",
        arguments = cmd,
        executable = ctx.executable._compiler,
    )

    return [DefaultInfo(files = depset(ctx.outputs.outs))]

_lcmgen = rule(
    attrs = {
        "src": attr.label(
            allow_single_file = True,
            mandatory = True,
        ),
        "_compiler": attr.label(
            executable = True,
            cfg = "exec",
            default = Label("@rules_symforce//symforce:lcmgen"),
        ),
        "outs": attr.output_list(),
    },
    output_to_genfiles = True,
    implementation = _impl,
)

def cc_lcm_library(name, srcs, deps = []):
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
    all_outputs["lcmtypes/symforce_types.lcm"] = ["lcmtypes/sym/type_t.hpp"]

    for lcmfile in srcs:
        basename = lcmfile.split(".")[0]
        _lcmgen(
            name = basename + "_lcm_gen",
            src = lcmfile,
            outs = all_outputs[lcmfile],
        )
    all_hdrs = []
    for v in all_outputs.values():
        all_hdrs.extend(v)
    native.cc_library(
        name = name,
        hdrs = all_hdrs,
        deps = ["@symforce//:skymarshal_core"] + deps,
        includes = [".", "lcmtypes"],
        visibility = ["//visibility:public"],
    )
