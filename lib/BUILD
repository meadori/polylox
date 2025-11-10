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
        "//include/llox:liblox_headers",
    ],
)
