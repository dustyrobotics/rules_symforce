
def _generate_requirements_impl(repository_ctx):
    # Read the template file
    template_file = repository_ctx.read(repository_ctx.attr.template)

    # Perform template substitution
    substituted_content = template_file.replace("file:", "#file:")
    #.replace("ruff==0.3.2", "ruff==0.3.4")

    # Write the substituted content to the output file
    repository_ctx.file("requirements.txt", substituted_content)
    # Create the BUILD file
    repository_ctx.file("BUILD", "exports_files(['requirements.txt'])")

generate_requirements = repository_rule(
    implementation = _generate_requirements_impl,
    attrs = {
        "template": attr.label(mandatory = True, allow_single_file = True),
    },
)
