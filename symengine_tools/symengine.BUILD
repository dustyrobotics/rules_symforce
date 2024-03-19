package(default_visibility = ["//visibility:public"])

load("@rules_symforce//symengine_tools:config.bzl", "symengine_config", "benchmark")

symengine_config(
     name="config",
     template = "symengine/symengine_config.h.in",
     output = "symengine/symengine_config.h",
)

filegroup(
    name="config_template",
    srcs = ["symengine/symengine_config.h.in"],
)

COPTS = [   
         "-std=c++11",   
         "-Wno-unused-but-set-variable",
         "-Wno-unused-variable",
]

cc_library(
        name = "symengine",
        hdrs = ["symengine/add.h",
"symengine/basic.h",
"symengine/basic-inl.h",
"symengine/basic-methods.inc",
"symengine/complex_double.h",
"symengine/complex.h",
"symengine/complex_mpc.h",
"symengine/constants.h",
"symengine/cwrapper.h",
"symengine/data_buffer_element.h",
"symengine/derivative.h",
"symengine/dict.h",
"symengine/diophantine.h",
"symengine/eval_arb.h",
"symengine/eval_double.h",
"symengine/eval.h",
"symengine/eval_mpc.h",
"symengine/eval_mpfr.h",
"symengine/expression.h",
"symengine/fields.h",
"symengine/finitediff.h",
"symengine/flint_wrapper.h",
"symengine/functions.h",
"symengine/infinity.h",
"symengine/integer.h",
"symengine/lambda_double.h",
"symengine/llvm_double.h",
"symengine/logic.h",
"symengine/matrix.h",
"symengine/monomials.h",
"symengine/mp_class.h",
"symengine/mp_wrapper.h",
"symengine/mul.h",
"symengine/nan.h",
"symengine/ntheory.h",
"symengine/number.h",
"symengine/parser.h",
"symengine/parser/parser.h",
"symengine/parser/parser.tab.hh",
"symengine/parser/parser_stype.h",
"symengine/parser/tokenizer.h",
"symengine/polys/basic_conversions.h",
"symengine/polys/cancel.h",
"symengine/polys/uexprpoly.h",
"symengine/polys/uintpoly_flint.h",
"symengine/polys/uintpoly.h",
"symengine/polys/uintpoly_piranha.h",
"symengine/polys/upolybase.h",
"symengine/polys/uratpoly.h",
"symengine/polys/usymenginepoly.h",
"symengine/polys/msymenginepoly.h",
"symengine/pow.h",
"symengine/printers/codegen.h",
"symengine/printers/mathml.h",
"symengine/printers/strprinter.h",
"symengine/printers/latex.h",
"symengine/printers.h",
"symengine/rational.h",
"symengine/real_double.h",
"symengine/real_mpfr.h",
"symengine/rings.h",
"symengine/series_flint.h",
"symengine/series_generic.h",
"symengine/series.h",
"symengine/series_piranha.h",
"symengine/series_visitor.h",
"symengine/sets.h",
"symengine/solve.h",
"symengine/subs.h",
"symengine/symbol.h",
"symengine/symengine_assert.h",
"symengine/symengine_casts.h",
"symengine/symengine_exception.h",
"symengine/symengine_rcp.h",
"symengine/type_codes.inc",
"symengine/visitor.h",
"symengine/test_visitors.h",
#strange includes here:
"symengine/as_real_imag.cpp",
"symengine/numer_denom.cpp",
],
        srcs = [
"symengine/mp_wrapper.cpp",
"symengine/add.cpp",
"symengine/basic.cpp",
"symengine/complex.cpp",
"symengine/complex_double.cpp",
"symengine/constants.cpp",
"symengine/cse.cpp",
"symengine/cwrapper.cpp",
"symengine/data_buffer_element.cpp",
"symengine/dense_matrix.cpp",
"symengine/derivative.cpp",
"symengine/dict.cpp",
"symengine/diophantine.cpp",
"symengine/eval.cpp",
"symengine/eval_double.cpp",
"symengine/expand.cpp",
"symengine/expression.cpp",
"symengine/fields.cpp",
"symengine/finitediff.cpp",
"symengine/functions.cpp",
"symengine/infinity.cpp",
"symengine/integer.cpp",
"symengine/logic.cpp",
"symengine/matrix.cpp",
"symengine/monomials.cpp",
"symengine/mul.cpp",
"symengine/nan.cpp",
"symengine/ntheory.cpp",
"symengine/number.cpp",
"symengine/parser/parser_old.cpp",
"symengine/parser/parser.tab.cc",
"symengine/parser/parser.cpp",
"symengine/parser/tokenizer.cpp",
"symengine/polys/basic_conversions.cpp",
"symengine/polys/msymenginepoly.cpp",
"symengine/polys/uexprpoly.cpp",
"symengine/polys/uintpoly.cpp",
"symengine/polys/uratpoly.cpp",
"symengine/pow.cpp",
"symengine/printers/codegen.cpp",
"symengine/printers/mathml.cpp",
"symengine/printers/strprinter.cpp",
"symengine/printers/latex.cpp",
"symengine/rational.cpp",
"symengine/real_double.cpp",
"symengine/rewrite.cpp",
"symengine/rings.cpp",
"symengine/series.cpp",
"symengine/series_generic.cpp",
"symengine/sets.cpp",
"symengine/solve.cpp",
"symengine/sparse_matrix.cpp",
"symengine/symbol.cpp",
"symengine/symengine_rcp.cpp",
"symengine/visitor.cpp",
#"symengine/test_visitors.cpp",
        ]
        +["symengine/symengine_config.h"],
   includes = [".","symengine"],
   deps=[
           #            "@boost//:multiprecision"
   ],
   defines = [
        # try static
        #        'SYMENGINE_STATIC_DEFINE=""',
        'SYMENGINE_EXPORT=""',
        #        'SYMENGINE_NO_EXPORT=""',
   ],
   copts=COPTS,
   linkopts=["-lgmp"] # find and compile this statically?
)

cc_library(name="bench_lib",
    hdrs = [
        "benchmarks/nonius.h++",
    ],
    srcs = [],
    includes=["benchmarks"],
)

cc_library(name="catch",
    hdrs = [
            #        "symengine/utilities/catch/catch.hpp",
        "symengine/symengine_config.h",
        "symengine/symengine_rcp.h",
        "symengine/symengine_assert.h",
    ],
    srcs = [
        "symengine/utilities/catch/catch.cpp",
    ],
    includes=[".","symengine/utilities/catch"],
    linkstatic = True,
    deps = [
        "@catch1",
   ],
)

cc_test(
    name = "test",
    srcs=[
        "symengine/tests/basic/test_as_numer_denom.cpp",
        "symengine/tests/basic/test_basic.cpp", 
        "symengine/tests/basic/test_cse.cpp",
        "symengine/tests/basic/test_functions.cpp",
        "symengine/tests/basic/test_integer_class.cpp",
        "symengine/tests/basic/test_nan.cpp",
        "symengine/tests/basic/test_parser.cpp",
        "symengine/tests/basic/test_rational.cpp",
        "symengine/tests/basic/test_series.cpp",
        "symengine/tests/basic/test_series_generic.cpp",
        "symengine/tests/basic/test_solve.cpp",
        "symengine/tests/basic/test_test_visitors.cpp",
        "symengine/tests/basic/test_arit.cpp",
        "symengine/tests/basic/test_as_real_imag.cpp",   
        "symengine/tests/basic/test_count_ops.cpp", 
        "symengine/tests/basic/test_fields.cpp",
        "symengine/tests/basic/test_infinity.cpp",
        "symengine/tests/basic/test_integer.cpp",      
        "symengine/tests/basic/test_number.cpp",
        "symengine/tests/basic/test_poly.cpp",
        "symengine/tests/basic/test_relationals.cpp",
        #"symengine/tests/basic/test_series_expansion_UP.cpp", uses piranha
        #"symengine/tests/basic/test_series_expansion_URatP.cpp", uses piranha
        #"symengine/tests/basic/test_series_expansion_URatF.cpp", # is flint. ignoring
        "symengine/tests/basic/test_sets.cpp",          
        "symengine/tests/basic/test_subs.cpp",
    ],
    deps=[
        ":symengine",
        ":catch",
    ],
    linkstatic = True,
)
 


# enable some benchmarks
benchmark("add1")
benchmark("bench" )
benchmark("bench_eval_double")
benchmark("diff_cache")
benchmark("eval_double1")
benchmark("expand1")
#benchmark("expand1.nb
benchmark("expand2b")
benchmark("expand2")
#benchmark("expand2_ginac")
          #benchmark("expand2.m
          #benchmark("expand2.mpl
benchmark("expand3")
          #benchmark("expand4.nb
          #benchmark("expand5.nb
#benchmark("expand6b")
#benchmark("expand6b_ginac")
          #benchmark("expand6b.m
          #benchmark("expand6b.mpl
benchmark("expand6")
#benchmark("expand6_ginac")
          #benchmark("expand6.m
          #benchmark("expand6.mpl
benchmark("expand7")
#benchmark("expand7_ginac")
          #benchmark("expand7.m
#benchmark("expand7.mpl
benchmark("lwbench")
#benchmark("lwbench_ginac")
benchmark("matrix_add1")
#benchmark("matrix_add1_ginac")
benchmark("matrix_add2")
#benchmark("matrix_add2_ginac")
benchmark("matrix_mul1")
#benchmark("matrix_mul1_ginac")
benchmark("matrix_mul2")
#benchmark("matrix_mul2_ginac")
#benchmark("nonius.h++
benchmark("ntheorybench")
benchmark("parsing")
benchmark("series")
#benchmark("series_expansion_sincos_flint")
#benchmark("series_expansion_sincos_piranha")
benchmark("series_expansion_sinp")
benchmark("symbench")
#benchmark("symbench_ginac")
#benchmark("symbench.m
#benchmark("symbench.nb
#benchmark("symengine_bench")
#benchmark("symengine_bench_ginac")
#benchmark("symengine_bench.m
#benchmark("symengine_bench.mpl
#benchmark("visitor_call")

