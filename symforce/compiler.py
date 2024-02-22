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
@click.option('--function',  type=str, help = "which function name should we compile")
@click.option('--function_is_generator', is_flag=True, help="is the function a function generator?")
@click.option('--arguments',  multiple=True, help="which arguments of the function should we linearize around")
@click.option('--output_names',  multiple=True, help="output the names of the arguments")
@click.option('--output_function_path', help="output path for function (eg residual.h ) file")
@click.option('--output_factor_path', default = None, help="output path for factor.h file")
def compile(input_path, function, function_is_generator, arguments, output_names, output_function_path, output_factor_path):
    import sympy

    print("Starting symforce codegen from {}", input_path)
    # read the python file from disk and execute it so the function is in the global namespace
    exec(open(input_path,"r").read(), globals())

    data = None

    if(function_is_generator):
        print("function is generator, calling it!!!!!!!!!!!!!!!!!!!!")
        #function = function()
        globals()[function] = globals()[function]()

    if(output_names):
        data = codegen.Codegen.function(globals()[function], 
                                    config=codegen.CppConfig(),
                                    name = function,
                                    output_names = output_names
                                    )
    else:
        data = codegen.Codegen.function(globals()[function], 
                                    config=codegen.CppConfig(),
                                    name = function,
                                    )

    # generate the residual 
    generated_function = data.generate_function()
    open(output_function_path,"w").write(open(generated_function.generated_files[0]).read())
    print("function: {}".format(function))
    print("to {}".format(function, output_function_path))

    if output_factor_path:
        # also generate the linearization function (eg the factor)
        # and write it to disk
        codegen_linearization = data.with_linearization(
            which_args = arguments
        )

        metadata_factor = codegen_linearization.generate_function()
        open(output_factor_path,"w").write(open(metadata_factor.generated_files[0]).read())
        print("factor:   {} to {}".format(function, output_factor_path))

if __name__ == "__main__":
    cli()
