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
@click.option('--basename',  type=str, help = "base name for residual function, and factor")
@click.option('--input_path',  help="input symforce python file")
@click.option('--function_name',  type=str, help = "which function name should we compile")
@click.option('--function_is_generator', is_flag=True, help="is the function a function generator?")
@click.option('--arguments',  multiple=True, help="which arguments of the function should we linearize around")
@click.option('--output_names',  multiple=True, help="output the names of the arguments")
@click.option('--output_function_path', help="output path for function (eg residual.h ) file")
@click.option('--output_factor_path', default = None, help="output path for factor.h file")
def compile(basename, input_path, function_name, function_is_generator, arguments, output_names, output_function_path, output_factor_path):
    import sympy

    print("Starting symforce codegen from {}".format(input_path))
    # read the python file from disk and execute it so the function is in the global namespace
    exec(open(input_path,"r").read(), globals())
    # get the function from the global namespace
    FUNC = globals()[function_name]
    
    data = None
    input_types = None

    if(function_is_generator):
        input_types, FUNC = FUNC()

    if output_factor_path:
        # if this is a factor the base function is FooFactorResidual
        # do that by appending snake case
        basename = basename + "_residual"

    if(output_names):
        data = codegen.Codegen.function(FUNC, 
                                    config=codegen.CppConfig(),
                                    name = basename,
                                    input_types = input_types,
                                    output_names = output_names
                                    )
    else:
        data = codegen.Codegen.function(FUNC, 
                                    config=codegen.CppConfig(),
                                    name = basename,
                                    input_types = input_types,
                                    )

    # generate the residual 
    generated_function = data.generate_function()
    open(output_function_path,"w").write(open(generated_function.generated_files[0]).read())
    print("function: {} -> codegen with base name: {}".format(function_name, basename))

    if output_factor_path:
        #basename_residual -> basename_factor when we output a factor
        # also generate the linearization function (eg the factor)
        # and write it to disk
        codegen_linearization = data.with_linearization(
            which_args = arguments
        )

        metadata_factor = codegen_linearization.generate_function()
        open(output_factor_path,"w").write(open(metadata_factor.generated_files[0]).read())
        print("function: {}".format(output_function_path))
        print("factor:   {}".format(output_factor_path))
    else:
        # this mode is just building a codegen py -> cc function. this is the path the residual function takes above
        print("function: {}".format(output_function_path))

if __name__ == "__main__":
    cli()
