import sys

import click
import symforce

symforce.set_epsilon_to_symbol()
# required for codegenning out cpp code!!!
symforce.set_symbolic_api("symengine")
symforce.set_log_level("warning")

import sympy

from symforce import codegen
from symforce.codegen import codegen_util
from symforce import geo
from symforce import ops
import symforce.symbolic as sf
from symforce import typing as T
from symforce.values import Values

@click.group()
def cli():
    pass

@cli.command()
@click.option('--input_path',  help="input symforce python file")
@click.option('--function',  help="which function should we compile")
@click.option('--arguments',  multiple=True, help="which arguments of the function should we linearize around")
@click.option('--output_names',  multiple=True, help="output the names of the arguments")
@click.option('--output_residual',  help="output path for residual.h file")
@click.option('--output_factor', default=None, help="output path for factor.h file")
def compile(input_path, function, arguments, output_names, output_residual, output_factor):
    import sympy

    # TODO sanitize this input better
    # using globals to evaluate the imports in the file
    exec(open(input_path,"r").read(), globals())

    data = None

    if(output_names):
        data = codegen.Codegen.function(globals()[function], 
                                    config=codegen.CppConfig(),
                                    output_names = output_names
                                    )
    else:
        data = codegen.Codegen.function(globals()[function], 
                                    config=codegen.CppConfig(),
                                    )

    # generate the residual 
    metadata_residual = data.generate_function()
    open(output_residual,"w").write(open(metadata_residual.generated_files[0]).read())

    if output_factor:
        codegen_linearization = data.with_linearization(
            which_args = arguments
        )

        metadata_factor = codegen_linearization.generate_function()
        open(output_factor,"w").write(open(metadata_factor.generated_files[0]).read())

    print("Compiled symforce codegen {} to {} and {}".format(input_path, output_residual, output_factor))

if __name__ == "__main__":
    cli()
