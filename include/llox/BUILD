load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "liblox_headers",
    hdrs = [
        "ast.h",
        "ast-printer.h",
        "environment.h",
        "interpreter.h",
        "object.h",
        "parser.h",
        "scanner.h",
        "token.h",
        "util.h",
    ],
    include_prefix = "llox",
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
)
