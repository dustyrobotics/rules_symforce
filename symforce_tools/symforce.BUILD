load("@rules_symforce//symforce_tools:gen.bzl", "cc_gen_pkg", "cc_sym_util_pkg", "lcm_pkg", "py_gen_pkg")
load("@rules_symforce//symforce_tools:lcmgen.bzl", "cc_lcm_library")
load("@symforce_requirements//:requirements.bzl", "requirement")

package(default_visibility = ["//visibility:public"])

COPTS = ["-std=c++17"]

cc_library(
    name = "skymarshal_core",
    hdrs = [
        "third_party/skymarshal/include/lcm/lcm_coretypes.h",
        "third_party/skymarshal/include/lcm/lcm_cpptypes.hpp",
        "third_party/skymarshal/include/lcm/lcm_reflection.hpp",
        "third_party/skymarshal/include/lcm/lcm_reflection_eigen.hpp",
    ],
    copts = COPTS,
    defines = ["SKYMARSHAL_PRINTING_ENABLED"],
    includes = ["third_party/skymarshal/include"],
)

cc_library(
    name = "eigen_lcm",
    hdrs = glob([
        "third_party/eigen_lcm/lcmtypes/eigen_lcm_lcm/cpp/lcmtypes/eigen_lcm/*.hpp",
    ]),
    copts = ["-std=c++17"],
    includes = ["third_party/eigen_lcm/lcmtypes/eigen_lcm_lcm/cpp/"],
)

cc_library(
    name = "symforce",
    srcs = glob(
        [
            "symforce/**/*.h",
            "symforce/**/*.hpp",
            "symforce/**/*.tcc",
            "symforce/**/*.cc",
        ],
        exclude = [
            "symforce/opt/**",
            "symforce/pybind/**",
            "symforce/benchmarks/**",
            "symforce/examples/**",
            "symforce/codegen/backends/**",
        ],
    ),
    copts = COPTS,
    includes = ["."],
    linkstatic = True,
    deps = [
        ":gen_lib",
        ":opt",
        "@eigen",
        "@spdlog",
    ],
)

cc_lcm_library(
    name = "symforce_lcm_lib",
    src = "lcmtypes/symforce.lcm",
)

cc_lcm_library(
    name = "symforce_types_lcm_lib",
    src = ":symforce_types",
)

py_library(
    name = "py",
    srcs = glob(
        [
            "symforce/**/*.py",
        ],
        exclude = [
            "symforce/codegen/backends/*/templates/**",
            # this is only used for initial import of the backend configs:
            # they are overridden in all tools with the rules_symforce ones
            "symforce/examples/**",
            "symforce/benchmarks/**",
        ],
    ),
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
GEO_TYPES = [
    "sf.Rot2",
    "sf.Rot3",
    "sf.Pose2",
    "sf.Pose3",
    "sf.Unit3",
]

GEO_FACTORS_TYPES = [
    "sf.Rot2",
    "sf.Rot3",
    "sf.V3",
    "sf.Pose2",
    "sf.Pose3",
]

CAM_TYPES = [
    "symforce.cam.atan_camera_cal.ATANCameraCal",
    "symforce.cam.double_sphere_camera_cal.DoubleSphereCameraCal",
    "symforce.cam.equirectangular_camera_cal.EquirectangularCameraCal",
    "symforce.cam.linear_camera_cal.LinearCameraCal",
    "symforce.cam.polynomial_camera_cal.PolynomialCameraCal",
    "symforce.cam.spherical_camera_cal.SphericalCameraCal",
]

# run codegen generators for the gen/ directory

# this generates the symforce_sym package
# it needs access to an interpreter that doesn't have sym already
# all other gen packages use this as a dep.
# it should be setup first
py_gen_pkg(
    name = "symforce_sym",
    cam_types = CAM_TYPES,
    geo_types = GEO_TYPES,
)

# the folowing two are the cc versions
cc_gen_pkg(
    name = "geo_package",
    geo_types = GEO_TYPES,
)

cc_gen_pkg(
    name = "cam_package",
    cam_types = CAM_TYPES,
    geo_types = GEO_TYPES,
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
    cam_types = CAM_TYPES,
    geo_types = GEO_TYPES,
)

lcm_pkg(
    name = "symforce_types",
    cam_types = CAM_TYPES,
    geo_types = GEO_TYPES,
)

#lcm_pkg(
#        name = "types",
#        # just the base symforce type gen
#)

cc_library(
    name = "gen_lib",
    srcs = [
        ":cam_package",
        ":geo_factors",
        ":geo_package",
        ":slam_factors",
        ":sym_util_package",
    ],
    hdrs = [
        ":cam_package",
        ":geo_factors",
        ":geo_package",
        ":slam_factors",
        ":sym_util_package",
    ],
    copts = COPTS,
    deps = [
        ":cam_package",
        ":geo_factors",
        ":geo_package",  # provides headers and includes
        ":skymarshal_core",
        ":slam_factors",
        ":sym_util_package",
        ":symforce_lcm_lib",
        ":symforce_types_lcm_lib",
        "@eigen",
    ],
)

# values needs to know about all the types that are genned
# this allows for injection of user defined types here
cc_library(
    name = "values",
    srcs = [
        "symforce/opt/key.cc",
        "symforce/opt/values.cc",
        "symforce/opt/values.tcc",
    ],
    hdrs = [
        "symforce/opt/assert.h",
        "symforce/opt/internal/hash_combine.h",
        "symforce/opt/key.h",
        "symforce/opt/optional.h",
        "symforce/opt/values.h",
    ],
    copts = COPTS,
    includes = ["."],
    deps = [
        ":gen_lib",
        "@fmt",
        "@tl_optional",
    ],
)

cc_library(
    name = "opt",
    srcs = glob(
        [
            "symforce/opt/**/*.cc",
        ],
        exclude = [
            "symforce/opt/sparse_cholesky/**",
            "symforce/opt/values.cc",
        ],
    ),
    hdrs = glob(
        [
            "symforce/opt/**/*.h",
            "symforce/opt/**/*.tcc",
        ],
        exclude = [
            "symforce/opt/sparse_cholesky/**",
            "symforce/opt/values.h",
            "symforce/opt/values.tcc",
        ],
    ),
    copts = COPTS,
    includes = ["."],
    deps = [
        ":eigen_lcm",
        ":skymarshal_core",
        ":sparse_cholesky",
        ":values",
        "@eigen",
        "@fmt",
        "@metis",
        "@spdlog",
        "@tl_optional",
    ],
)

cc_library(
    name = "sparse_cholesky",
    srcs = glob([
        "symforce/opt/sparse_cholesky/**/*.cc",
        "symforce/opt/sparse_cholesky/**/*.tcc",
    ]),
    hdrs = glob([
        "symforce/opt/sparse_cholesky/**/*.h",
        "symforce/opt/assert.h",
    ]),
    copts = COPTS,
    deps = [
        "@eigen",
        "@fmt",
        "@metis",
        "@spdlog",
    ],
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

cc_test(
    name = "symforce_factor_test",
    srcs = ["test/symforce_factor_test.cc"],
    deps = [
        #":lcmtypes",
        ":symforce",
        "@catch2//:catch2_main",
    ],
)

py_library(
    name = "skymarshal",
    srcs = glob(["third_party/skymarshal/skymarshal/**/*.py"]),
    data = glob(["third_party/skymarshal/**"]),
    imports = ["third_party/skymarshal"],
    deps = [
        requirement("jinja2"),
        requirement("numpy"),
        requirement("ply"),
        requirement("six"),
    ],
)

py_binary(
    name = "skymarshalpy",
    srcs = ["third_party/skymarshal/skymarshal/__main__.py"],
    main = "third_party/skymarshal/skymarshal/__main__.py",
    deps = [
        ":skymarshal",
    ],
)
