
import click

from symforce_repo.symforce_sym_gen.python import sym

import symforce
symforce.set_epsilon_to_symbol()
# required for codegenning out cpp code!!!
symforce.set_symbolic_api("symengine")
symforce.set_log_level("warning")
from symforce import codegen

from rules_symforce.symforce_tools.codegen.backends.cpp.cpp_config import CppConfig

def write_function_to_disk(data, path):
    # generate the residual 
    generated_function = data.generate_function()
    open(path, "w").write(open(generated_function.generated_files[0]).read())

    
def write_linearization_to_disk(codegen_linearization, path):
    metadata_factor = codegen_linearization.generate_function()
    open(path, "w").write(open(metadata_factor.generated_files[0]).read())


def read_python_source(input_path, function_name):
    # read the python file from disk and execute it so the python code we want is in the global namespace
    exec(open(input_path,"r").read(), globals())
    # we generate from a single function, so we need to get it from the global namespace
    return globals()[function_name]

@click.group()
def cli():
    pass

@cli.command()
@click.option('--basename',  type=str, required=True, help = "base name for residual function, and factor")
@click.option('--function_name',  type=str, help = "which function name should we compile")
@click.option('--function_is_generator', is_flag=True, help="is the function a function generator?")
@click.option('--arguments',  multiple=True, help="which arguments of the function should we linearize around")
@click.option('--output_names',  multiple=True, help="output the names of the arguments")
@click.option('--python_src',  help="input symforce python file")
@click.option('--output_residual_cpp', help="output path for residual function  (eg basename_residual.h ) ")
@click.option('--output_factor_cpp', default = None, help="output path for factor.h file")
def generatefactor(basename, 
                    python_src, 
                    function_name, 
                    function_is_generator, 
                    arguments, 
                    output_names, 
                    output_residual_cpp, 
                    output_factor_cpp):
    print("Starting symforce factor codegen from {}".format(python_src))
    from symforce_repo.symforce_sym_gen.python import sym

    FUNCTION = read_python_source(python_src, function_name)

    input_types = None
    if len(output_names) == 0:
        output_names = None 
    # read the input types in a workaround to read the annotations
    if(function_is_generator):
        input_types, FUNCTION = FUNCTION()

    # FooFactorResidual
    # factors generate two files, the residual and the factor
    # factors have BaseNameResidual and BaseNameFactor as the names
    # do that by appending snake case to the 
    name = basename + "_residual"


    data = codegen.Codegen.function(FUNCTION, 
                                    config=CppConfig(),
                                    name = name,
                                    input_types = input_types,
                                    output_names = output_names
                            )    

    write_function_to_disk(data, output_residual_cpp)
    print("function: {}".format( output_residual_cpp))

    print("linearization arguments:", arguments)
    # also generate the linearization function (eg the factor)
    # it is linearized around "which_args"
    codegen_linearization = data.with_linearization(
            which_args = arguments
    )

    write_linearization_to_disk(codegen_linearization, output_factor_cpp)
    print("factor:   {}".format(output_factor_cpp))



@cli.command()
@click.option('--basename',  type=str, required=True, help = "base name for the generated function(s)")
@click.option('--python_src',  help="input symforce python file to be evaluated")
@click.option('--function_name',  type=str, help = "which function name should we compile")
@click.option('--return_key',  type=str, default=None, help = "return key object for function, eg Pose3")
@click.option('--output_names',  multiple=True, type=str, default=None, help="the names of the output args")
@click.option('--output_cpp', help="output path for function (eg residual.h ) file")
def rawcodegen(basename, python_src, function_name, return_key, output_names, output_cpp):
    print("Starting symforce codegen from {}".format(python_src))
    
    FUNCTION = read_python_source(python_src, function_name)


    if output_names == ():
        output_names = None 
    # try and read the input types from the function interface
    input_types = None

    print("the return key for this is:", return_key)
    data = codegen.Codegen.function(FUNCTION, 
                                    config=CppConfig(),
                                    name = basename,
                                    input_types = input_types,
                                    output_names = output_names,
                                    return_key = return_key,
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



    print("function: {} -> codegen with base name: {}".format(function_name, basename))
    write_function_to_disk(data, output_cpp)
    print("function: {}".format( output_cpp))


if __name__ == "__main__":
    cli()
