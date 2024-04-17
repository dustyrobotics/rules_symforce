package(default_visibility = ["//visibility:public"])

load("@symforce_requirements//:requirements.bzl", "requirement")
load("@rules_symforce//symforce_tools:lcmgen.bzl", "cc_lcm_library")
load("@rules_symforce//symforce_tools:gen.bzl", "cc_gen_pkg", "cc_sym_util_pkg", "lcm_pkg", "py_gen_pkg")

COPTS = ["-std=c++17"]

cc_library(
    name = "skymarshal_core",
    hdrs = [
        "third_party/skymarshal/include/lcm/lcm_cpptypes.hpp",
        "third_party/skymarshal/include/lcm/lcm_coretypes.h",
        "third_party/skymarshal/include/lcm/lcm_reflection_eigen.hpp",
        "third_party/skymarshal/include/lcm/lcm_reflection.hpp",
      ],
    defines=["SKYMARSHAL_PRINTING_ENABLED"],
    includes=["third_party/skymarshal/include"],
    copts=COPTS,
)

cc_library(
    name = "eigen_lcm",
    hdrs = glob([
       "third_party/eigen_lcm/lcmtypes/eigen_lcm_lcm/cpp/lcmtypes/eigen_lcm/*.hpp"
      ]),
    includes=["third_party/eigen_lcm/lcmtypes/eigen_lcm_lcm/cpp/"],
    copts=["-std=c++17"],
)

cc_library(
    name = "symforce",
    srcs = glob([
        "symforce/**/*.h",
        "symforce/**/*.hpp",
        "symforce/**/*.tcc",
        "symforce/**/*.cc",
    ],   exclude=[
          "symforce/opt/**",
          "symforce/pybind/**",
          "symforce/benchmarks/**",
          "symforce/examples/**",
          "symforce/codegen/backends/**",
     ]),
    includes = ["."],
    deps=[
        "@spdlog",
        "@eigen",
        ":opt",
        ":gen_lib",
    ],
    linkstatic = True,
    copts=COPTS,
)

cc_lcm_library(
        name="symforce_lcm_lib",
        src = "lcmtypes/symforce.lcm",
)

cc_lcm_library(
        name="symforce_types_lcm_lib",
        src = ":symforce_types",
)

py_library(
    name = "py",
    srcs = glob([
        "symforce/**/*.py",
    ],
     exclude=[
        "symforce/codegen/backends/*/templates/**",
        # this is only used for initial import of the backend configs:
        # they are overridden in all tools with the rules_symforce ones
        "symforce/examples/**",
        "symforce/benchmarks/**",
     ]),
    deps = [
        requirement("sympy"),
        requirement("numpy"),
        requirement("jinja2"),
        requirement("typing-extensions"),
        "@symenginepy",
    ],
)

# the python bindings all filled out with generated types as well
# with sym
py_library(
        name = "symforce_python",
        deps = [
            ":py",
            ":symforce_sym",
            "@symenginepy",
        ],
)

# all the registered GEO TYPES
GEO_TYPES = ["sf.Rot2", "sf.Rot3", "sf.Pose2", "sf.Pose3", "sf.Unit3"]
GEO_FACTORS_TYPES = ["sf.Rot2", "sf.Rot3", "sf.V3", "sf.Pose2", "sf.Pose3"]

CAM_TYPES = [   
        'symforce.cam.atan_camera_cal.ATANCameraCal', 
        'symforce.cam.double_sphere_camera_cal.DoubleSphereCameraCal', 
        'symforce.cam.equirectangular_camera_cal.EquirectangularCameraCal', 
        'symforce.cam.linear_camera_cal.LinearCameraCal',  
        'symforce.cam.polynomial_camera_cal.PolynomialCameraCal', 
        'symforce.cam.spherical_camera_cal.SphericalCameraCal']

# run codegen generators for the gen/ directory

# this generates the symforce_sym package
# it needs access to an interpreter that doesn't have sym already
# all other gen packages use this as a dep.
# it should be setup first
py_gen_pkg( 
        name = "symforce_sym", 
        geo_types = GEO_TYPES,
        cam_types = CAM_TYPES,
)

# the folowing two are the cc versions
cc_gen_pkg( 
        name = "geo_package", 
        geo_types = GEO_TYPES,
)

cc_gen_pkg( 
        name = "cam_package", 
        geo_types = GEO_TYPES,
        cam_types = CAM_TYPES,
)



cc_gen_pkg( 
        name = "geo_factors", 
           # generate between factors for these types
        geo_types = GEO_FACTORS_TYPES,
)

cc_gen_pkg( 
        name = "slam_factors", 
        cam_types = CAM_TYPES,
)

cc_sym_util_pkg( 
        name = "sym_util_package", 
        geo_types = GEO_TYPES, 
        cam_types = CAM_TYPES,
)

lcm_pkg( 
        name = "symforce_types", 
        geo_types = GEO_TYPES, 
        cam_types = CAM_TYPES,
)

#lcm_pkg( 
#        name = "types", 
#        # just the base symforce type gen
#)

cc_library(
    name = "gen_lib",
    srcs = [
        ":geo_package",
        ":geo_factors",
        ":slam_factors",
        ":cam_package",
        ":sym_util_package",
    ],
    hdrs = [
        ":geo_package",
        ":geo_factors",
        ":slam_factors",
        ":cam_package",
        ":sym_util_package",
    ],
    deps = [
        ":geo_package", # provides headers and includes
        ":geo_factors",
        ":cam_package",
        ":slam_factors",
        ":sym_util_package",
        ":symforce_lcm_lib",
        ":symforce_types_lcm_lib",
        "@eigen",
        ":skymarshal_core"
    ],
    copts=COPTS,
)

# values needs to know about all the types that are genned
# this allows for injection of user defined types here   
cc_library(
    name = "values",
    srcs = [
       "symforce/opt/values.cc",
       "symforce/opt/values.tcc",
       "symforce/opt/key.cc",
    ],
    hdrs =  [
       "symforce/opt/values.h",
       "symforce/opt/assert.h",
       "symforce/opt/internal/hash_combine.h",
       "symforce/opt/key.h",
       "symforce/opt/optional.h",
    ],
    deps = [
        ":gen_lib",
        "@fmt",
        "@tl_optional",
    ],
    includes = ["."],
    copts=COPTS,
)

cc_library(
    name = "opt",
    srcs = glob([
       "symforce/opt/**/*.cc",
    ],
    exclude=[
       "symforce/opt/sparse_cholesky/**",
       "symforce/opt/values.cc",
    ]),
    hdrs =  glob([
        "symforce/opt/**/*.h",
        "symforce/opt/**/*.tcc",
      ],
        exclude=[
          "symforce/opt/sparse_cholesky/**",
          "symforce/opt/values.h",
          "symforce/opt/values.tcc",
     ]),
    deps = [
        ":sparse_cholesky",
        ":skymarshal_core",
        ":eigen_lcm",
        "@metis",
        "@eigen",
        "@spdlog",
        "@fmt",
        "@tl_optional",
        ":values",
    ],
    includes = ["."],
    copts=COPTS,
)

cc_library(
    name = "sparse_cholesky",
    srcs = glob([
       "symforce/opt/sparse_cholesky/**/*.cc",
       "symforce/opt/sparse_cholesky/**/*.tcc",
    ]),
    hdrs =  glob([
       "symforce/opt/sparse_cholesky/**/*.h",
       "symforce/opt/assert.h",
    ]),
    deps = [
        "@metis",
        "@eigen",
        "@spdlog",
        "@fmt",
    ],
    copts=COPTS,
)

#cc_test(
#        name ="test",
#        srcs = ["test/codegen_cpp_test.cc"],
#        deps = [
#            ":symforce",
#            ":lcmtypes",
#            "@catch2//:catch2_main",
#        ],
#)

py_library(
    name = "skymarshal",
    srcs = glob(["third_party/skymarshal/skymarshal/**/*.py"]),
    imports = ["third_party/skymarshal"],
    deps = [
            requirement("jinja2"),
            requirement("numpy"),
            requirement("ply"),
            requirement("six"),
    ],
    data = glob(["third_party/skymarshal/**"])
)

py_binary(
    name = "skymarshalpy",
    srcs = ["third_party/skymarshal/skymarshal/__main__.py"],
    main = "third_party/skymarshal/skymarshal/__main__.py",
    deps = [
            ":skymarshal",
    ],
)


