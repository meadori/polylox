load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "liblox",
    srcs = [
        "ast-printer.cpp",
        "interpreter.cpp",
        "parser.cpp",
        "scanner.cpp",
        "token.cpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//include/lox:liblox_headers",
    ],
)
