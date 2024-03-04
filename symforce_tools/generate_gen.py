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
#from symforce.codegen import cam_package_codegen
from symforce.codegen import geo_factors_codegen

from rules_symforce.symforce_tools.codegen import geo_package_codegen_stripped
from rules_symforce.symforce_tools.codegen import cam_package_codegen_stripped
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
@common_options
def lcm_types(output_dir):
 
    # LCM type_t
    # need to generate the lcm types
    templates.add(
        template_path="symforce_types.lcm.jinja",
        data= lcm_types_codegen.lcm_symforce_types_data(),
        config= config.render_template_config,
        template_dir= template_util.LCM_TEMPLATE_DIR,
        output_path= package_dir / ".." / "lcmtypes" / "lcmtypes" / "symforce_types.lcm",
    )

    templates.render()

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
    if hdrs:
        for f in cc_base.rglob("*.cc"):
            print("removing", f)
    if srcs:
        for f in cc_base.rglob("*.h"):
            print("removing", f)
    print_dir(cc_base)

@cli.command()
@common_options
def geo_factors(output_dir):
    TYPES = (sf.Rot2, sf.Rot3, sf.V3, sf.Pose2, sf.Pose3)
    cc_config = CppConfig()
    cc_base = output_dir / "cpp"
    geo_factors_codegen.generate_between_factors(types = TYPES, output_dir = cc_base / "sym" / "factors")
    geo_factors_codegen.generate_pose3_extra_factors(cc_base / "sym" / "factors" )

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
    if hdrs:
        for f in cc_base.rglob("*.cc"):
            print("removing", f)
    if srcs:
        for f in cc_base.rglob("*.h"):
            print("removing", f)
    print_dir(cc_base)


@cli.command()
@common_options
def slam_factors(output_dir):
    cc_config = CppConfig()
    cc_base = output_dir / "cpp"
    print("slam factors")
    slam_factors_codegen.generate(output_dir = cc_base / "sym", config = cc_config)
    print("imu factors")
    generate_manifold_imu_preintegration(
            config = cc_config,
            output_dir = cc_base / "sym" / "factors" / "internal",
    )

@cli.command()
@common_options
def generate(output_dir):
    TYPES = (sf.Rot2, sf.Rot3, sf.V3, sf.Pose2, sf.Pose3)
    py_config = PythonConfig()
    cc_config = CppConfig()
    cc_base = output_dir / "cpp"
    py_base = output_dir / "python"

    # Generate cam package, geo package
    # This calls geo_package_codegen.generate internally
    # First generate the geo package as it's a dependency of the cam package
    print("geo package")
    # also makes lcm types .lcm
    geo_package_codegen.generate(config = cc_config, output_dir = cc_base)
    #geo_package_codegen.generate(config = py_config, output_dir = py_base)

    #print("cam package")
    cam_package_codegen.generate(config = cc_config, output_dir = cc_base)
    #cam_package_codegen.generate(config = py_config, output_dir = py_base)
    
    ## Prior factors, between factors, and SLAM factors for C++.
    print("geo factors")
    ## geo factors codegen calls the following two functions
    geo_factors_codegen.generate_between_factors(types = TYPES, output_dir = cc_base / "sym" / "factors")
    geo_factors_codegen.generate_pose3_extra_factors(cc_base / "sym" / "factors" )
    
    print("slam factors")
    slam_factors_codegen.generate(output_dir = cc_base / "sym", config = cc_config)
    print("imu factors")
    generate_manifold_imu_preintegration(
            config = cc_config,
            output_dir = cc_base / "sym" / "factors" / "internal",
    )

    # LCM type_t
    # need to generate the lcm types
    #templates.add(
    #    template_path="symforce_types.lcm.jinja",
    #    data=lcm_types_codegen.lcm_symforce_types_data(),
    #    config=config.render_template_config,
    #    template_dir=template_util.LCM_TEMPLATE_DIR,
    #    output_path=package_dir / ".." / "lcmtypes" / "lcmtypes" / "symforce_types.lcm",
    #)

    #templates.render()
    # Generate typedefs.h
    #print("sym_util_package_codegen.generate")
    #sym_util_package_codegen.generate(config = cc_config, output_dir = cc_base)

    # filter out what we don't want
    print("filtering out non .h and .cc files")
    for f in cc_base.rglob("*.lcm"):
        f.unlink()
    for f in cc_base.rglob("*.cc"):
        if "tests" in f.parts:
            f.unlink()

if __name__ == "__main__":
    cli()
