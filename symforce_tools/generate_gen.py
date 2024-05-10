# ----------------------------------------------------------------------------
# SymForce - Copyright 2022, Skydio, Inc.
# This source code is under the Apache 2.0 license found in the LICENSE file.
# ----------------------------------------------------------------------------
from pathlib import Path

import click
import os

import symforce

print("imported symforce from", symforce.__file__)

symforce.set_epsilon_to_symbol()

import symforce.symbolic as sf


from symforce import typing as T

from rules_symforce.symforce_tools.codegen import geo_factors_codegen_stripped
from rules_symforce.symforce_tools.codegen import slam_factors_codegen_stripped
from rules_symforce.symforce_tools.codegen import geo_package_codegen_stripped
from rules_symforce.symforce_tools.codegen import cam_package_codegen_stripped
from rules_symforce.symforce_tools.codegen import types_package_codegen_stripped
from rules_symforce.symforce_tools.codegen import sym_util_package_codegen_stripped

from symforce.codegen import slam_factors_codegen
from symforce.slam.imu_preintegration.generate import generate_manifold_imu_preintegration

from rules_symforce.symforce_tools.codegen.backends.cpp.cpp_config import CppConfig
from rules_symforce.symforce_tools.codegen.backends.python.python_config import PythonConfig

def print_dir(directory):
    for root, dirs, files in os.walk(directory):
        for f in files:
            print( os.path.join(root, f))

def lookup_type(module, type_name):
    return getattr(module, type_name)

def lookup_type(type_str):
    module_name, type_name = type_str.rsplit(".", 1)
    #print( module_name, type_name)
    # try in the global namespace
    module = globals().get(module_name)
    if module is None:
        #print("module", module_name, "not found in global namespace")
        sub_t = lookup_type(module_name)
        if sub_t is not None:
            type_class = getattr(sub_t, type_name)
            #print(type_class)
            if type_class is not None:
                return type_class
        return None
    type_class = getattr(module, type_name)
    if type_class is not None:
        return type_class
    else:
        return None

@click.group()
def cli():
    pass

def common_options(f):
    f = click.option("--output_dir",  type=Path, required=True, help = "path to output directory")(f)
    return f

def cc_common_options(f):
    f = common_options(f)
    f = click.option("--hdrs",  type=bool, default=False, help = "only compile headers")(f)
    f = click.option("--srcs",  type=bool, default=False, help = "only compile headers")(f)
    return f

@cli.command()
@click.option('--geo_types',  multiple=True, help="what geo types are in the package to generate")
@click.option('--cam_types',  multiple=True, help="what camera types are in the package to generate")
@click.option("--output_lcm",  type=Path, required=True, help = "path to output file")
def symforce_types(output_lcm, geo_types, cam_types):
    from symforce.codegen import codegen_config
    from symforce.codegen import template_util
    from symforce import python_util
    from rules_symforce.symforce_tools.codegen.lcm_template_dir import LCM_TEMPLATE_DIR

    GEO_TYPES = gather_types(geo_types)
    CAM_TYPES = gather_types(cam_types)
    print("template dir", LCM_TEMPLATE_DIR)

    template_util.render_template(
            template_dir = LCM_TEMPLATE_DIR,
            template_path = "symforce_types.lcm.jinja",
            data= dict(python_util=python_util, 
                       GEO_TYPES = GEO_TYPES,
                       CAM_TYPES = CAM_TYPES,
                    ),
            config = codegen_config.RenderTemplateConfig(),
            #output_path = output_dir / "lcmtypes" / "symforce_types.lcm",
            output_path = output_lcm,
    )
    print("output", output_lcm)

@cli.command()
@click.option("--output_lcm",  type=Path, required=True, help = "path to output file")
def types(output_lcm):
    from symforce.codegen import codegen_config
    from symforce.codegen import template_util
    from symforce import types_util
    from rules_symforce.symforce_tools.codegen.lcm_template_dir import LCM_TEMPLATE_DIR

    # TODO re explore this when we want to generate types via values automatically
    # can't get this to work!!!
    # I am not sure how to generate symforce.lcm
    types_package_codegen_stripped.generate_types(
                                                package_name = "sym",
                                                file_name = output_lcm,
#    values_indices: T.Mapping[str, T.Dict[str, IndexEntry]],
                                                use_eigen_types = True,
#    shared_types: T.Mapping[str, str] = None,
                                            scalar_type = "double",
                                            output_dir = output_lcm.parent ,
#   lcm_bindings_output_dir: T.Openable = None,
                                           templates =  "types.lcm.jinja")
    types_to_generate = types_util.get_types_to_generate()
    template_util.render_template(
            template_dir = LCM_TEMPLATE_DIR,
            template_path = "types.lcm.jinja",
            data=dict(
               # data,
                types_to_generate = types_to_generate,
                types_util = types_util,
                use_eigen_types = True,
            ),
            config = codegen_config.RenderTemplateConfig(),
            output_path = output_lcm,
    )

@cli.command()
@common_options
@click.option('--geo_types',  multiple=True, help="what geo types are in the package to generate")
@click.option('--cam_types',  multiple=True, help="what camera types are in the package to generate")
def sym_util_package(output_dir, geo_types, cam_types):
    #cpp only
    cc_config = CppConfig()
    cc_base = output_dir / "cpp"
 
    # Generate typedefs.h
    # this needs access to all the types
    print("sym_util_package_codegen.generate to", cc_base)

    GEO_TYPES = gather_types(geo_types)
    CAM_TYPES = gather_types(cam_types)
    # cam types should be gathered as subclass of the given types
    #CAM_TYPES = sorted(sf.CameraCal.__subclasses__(), key=lambda cls: cls.__name__)
    print("geo types", GEO_TYPES)
    print("cam types", CAM_TYPES)
    sym_util_package_codegen_stripped.generate(GEO_TYPES, 
                                               CAM_TYPES, 
                                               config = cc_config, 
                                               output_dir = cc_base)
    print_dir(cc_base)

def gather_types(types: T.Sequence[str]):
    TYPES = []
    for type_str in types:
        TYPES.append(lookup_type(type_str))
    return TYPES

@cli.command()
@click.option('--geo_types',  multiple=True, help="what types are in the package to generate")
@cc_common_options
def geo_package(output_dir, geo_types, hdrs, srcs):
    print("gen geo package to", output_dir)
    #py_config = PythonConfig()
    cc_config = CppConfig()
    cc_base = output_dir / "cpp"
    #py_base = output_dir / "python"
    # lookup type in namespace
    GEO_TYPES = gather_types(geo_types)
    print("geo types", GEO_TYPES)
 
    geo_package_codegen_stripped.generate(GEO_TYPES = GEO_TYPES, 
                                          config = cc_config, 
                                          output_dir = cc_base)
    #split headers and cc files
    #if hdrs:
    #    for f in cc_base.rglob("*.cc"):
    #        print("removing", f)
    #if srcs:
    #    for f in cc_base.rglob("*.h"):
    #        print("removing", f)
    print_dir(cc_base)

@cli.command()
@click.option('--geo_types',  multiple=True, help="what geo types are in the package to generate")
@cc_common_options
def geo_factors(output_dir, geo_types, hdrs, srcs):
    cc_base = output_dir / "cpp"
    GEO_TYPES = gather_types(geo_types)
    geo_factors_codegen_stripped.generate_between_factors(types = GEO_TYPES, output_dir = cc_base / "sym" / "factors")
    geo_factors_codegen_stripped.generate_pose3_extra_factors(cc_base / "sym" / "factors" )
    print_dir(cc_base)

@cli.command()
@click.option('--geo_types',  multiple=True, help="what geo types are in the package to generate")
@click.option('--cam_types',  multiple=True, help="what camera types are in the package to generate")
@cc_common_options
def cam_package(output_dir, geo_types, cam_types, hdrs, srcs):
    print("gen cam package to", output_dir)
    #py_config = PythonConfig()
    cc_config = CppConfig()
    cc_base = output_dir / "cpp"
    #py_base = output_dir / "python"
    # lookup type in namespace
    GEO_TYPES = gather_types(geo_types)
    CAM_TYPES = gather_types(cam_types)
    print("geo types", GEO_TYPES)
    print("cam types", CAM_TYPES)
 
    cam_package_codegen_stripped.generate(GEO_TYPES = GEO_TYPES, 
                                 CAM_TYPES = CAM_TYPES, 
                                 config = cc_config, output_dir = cc_base)
    #split headers and cc files
    #if hdrs:
    #    for f in cc_base.rglob("*.cc"):
    #        print("removing", f)
    #if srcs:
    #    for f in cc_base.rglob("*.h"):
    #        print("removing", f)
    print_dir(cc_base)


@cli.command()
@common_options
@click.option('--geo_types',  multiple=True, help="what geo types are in the package to generate")
@click.option('--cam_types',  multiple=True, help="what camera types are in the package to generate")
def slam_factors(output_dir, geo_types,  cam_types):
    cc_config = CppConfig()
    cc_base = output_dir / "cpp"
    CAM_TYPES = gather_types(cam_types)
    print("slam factors")
    print("cam types", CAM_TYPES)
    slam_factors_codegen_stripped.generate(CAM_TYPES = CAM_TYPES,  
                                    output_dir = cc_base / "sym", 
                                    config = cc_config)
    print("imu factors")
    generate_manifold_imu_preintegration(
            config = cc_config,
            output_dir = cc_base / "sym" / "factors" / "internal",
    )
    print_dir(cc_base)

@cli.command()
@common_options
@click.option('--geo_types',  multiple=True, help="what geo types are in the package to generate")
@click.option('--cam_types',  multiple=True, help="what camera types are in the package to generate")
def symforce_sym(output_dir, geo_types, cam_types):
    """
    generate the python symforce-sym package
    """
    py_config = PythonConfig()
    py_base = output_dir / "python"

    GEO_TYPES = gather_types(geo_types)
    CAM_TYPES = gather_types(cam_types)
    print("geo package")
    geo_package_codegen_stripped.generate(GEO_TYPES = GEO_TYPES,
                                          config = py_config, 
                                          output_dir = py_base)

    print("cam package")
    cam_package_codegen_stripped.generate(GEO_TYPES = GEO_TYPES, 
                                        CAM_TYPES = CAM_TYPES, 
                                        config = py_config, 
                                        output_dir = py_base)
    print_dir(py_base)

if __name__ == "__main__":
    cli()
